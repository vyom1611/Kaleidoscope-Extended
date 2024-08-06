#ifndef CODEGEN_H
#define CODEGEN_H

#include "AST.h"
#include "llvm/IR/IRBuilder.h"
#include <memory>
#include <map>

using namespace llvm;

extern std::unique_ptr<LLVMContext> TheContext;
extern std::unique_ptr<Module> TheModule;
extern std::unique_ptr<IRBuilder<>> Builder;
extern std::map<std::string, AllocaInst *> NamedValues;
extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

void InitializeModule();
void CreateOptimizations(Module &M);

#endif // CODEGEN_H
