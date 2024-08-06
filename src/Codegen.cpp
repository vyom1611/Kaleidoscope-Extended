#include "../include/Codegen.h"

#include <llvm/IR/IRBuilder.h>

#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/OptimizationLevel.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/IPO/Attributor.h"
#include "llvm/Transforms/IPO/DeadArgumentElimination.h"
#include "llvm/Transforms/IPO/LowerTypeTests.h"
#include "llvm/Transforms/IPO/SampleProfileProbe.h"
#include "llvm/Transforms/Scalar/LoopDeletion.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Scalar/MemCpyOptimizer.h"
#include "llvm/Transforms/Scalar/NewGVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"


std::unique_ptr<LLVMContext> TheContext;
std::unique_ptr<Module> TheModule;
std::unique_ptr<IRBuilder<>> Builder;
std::map<std::string, AllocaInst *> NamedValues;
std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

void InitializeModule() {
    TheContext = std::make_unique<LLVMContext>();
    TheModule = std::make_unique<Module>("my cool jit", *TheContext);
    Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

void PrintIR(Module &M, const std::string &Message) {
    std::string IRString;
    llvm::raw_string_ostream IRStream(IRString);
    M.print(IRStream, nullptr);
    IRStream.flush();
    fprintf(stderr, "%s\n%s\n", Message.c_str(), IRString.c_str());
}

void CreateOptimizations(Module &M) {
    // Create the analysis managers.
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;

    // Create the new pass manager builder.
    PassBuilder PB;

    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    // Create and run each pass individually.
    FunctionPassManager FPM;

    // Get the function by name
    Function *FooFunc = M.getFunction("foo");
    if (!FooFunc) {
        fprintf(stderr, "Function 'foo' not found in the module.\n");
        return;
    }

    // SimplifyCFG
    FPM.addPass(SimplifyCFGPass());
    FPM.run(*FooFunc, FAM);
    PrintIR(M, "After SimplifyCFGPass:");

    FPM = FunctionPassManager();
    FPM.addPass(InstCombinePass());
    FPM.run(*FooFunc, FAM);
    PrintIR(M, "After InstCombinePass:");

    FPM = FunctionPassManager();
    FPM.addPass(ReassociatePass());
    FPM.run(*FooFunc, FAM);
    PrintIR(M, "After ReassociatePass:");

    FPM = FunctionPassManager();
    FPM.addPass(NewGVNPass());
    FPM.run(*FooFunc, FAM);
    PrintIR(M, "After NewGVNPass:");

}
