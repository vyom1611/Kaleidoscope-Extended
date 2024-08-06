#include "../include/Codegen.h"
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/KaleidoscopeJIT.h"
#include "../include/DebugInfo.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/IR/LegacyPassManager.h"
#include <iostream>
#include <llvm/IR/DIBuilder.h>
#include <llvm/Support/Error.h>

using namespace llvm;
using namespace llvm::orc;

std::unique_ptr<KaleidoscopeJIT> TheJIT;
ExitOnError ExitOnErr;

void InitializeDebugInfo() {
    DBuilder = std::make_unique<llvm::DIBuilder>(*TheModule);
    KSDbgInfo.TheCU = DBuilder->createCompileUnit(
        llvm::dwarf::DW_LANG_C, DBuilder->createFile("my cool jit", "."),
        "Kaleidoscope Compiler", 0, "", 0);
}

void HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
        if (!FnAST->codegen())
            fprintf(stderr, "Error reading function definition\n");
    } else {
        getNextToken();
    }
}

void HandleExtern() {
    if (auto ProtoAST = ParseExtern()) {
        if (!ProtoAST->codegen())
            fprintf(stderr, "Error reading extern\n");
        else
            FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    } else {
        getNextToken();
    }
}

void HandleTopLevelExpression() {
    if (auto FnAST = ParseTopLevelExpr()) {
        if (!FnAST->codegen())
            fprintf(stderr, "Error generating code for top level expr\n");
    } else {
        getNextToken();
    }
}

void MainLoop() {
    while (true) {
        std::cout << "ready> ";
        std::cout.flush(); // Ensure the prompt is displayed immediately
        switch (CurTok) {
        case tok_eof:
            return;
        case ';': // Ignore top-level semicolons.
            getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

int main() {
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    BinopPrecedence['='] = 2;
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;

    getNextToken();

    TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

    InitializeModule();
    InitializeDebugInfo();

    legacy::FunctionPassManager FPM(TheModule.get());
    CreateOptimizations(FPM);

    MainLoop();

    DBuilder->finalize(); // Finalize the DIBuilder

    TheModule->print(errs(), nullptr);

    return 0;
}
