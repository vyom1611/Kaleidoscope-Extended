#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include <vector>
#include "AST.h"
#include "SourceLocation.h"

struct DebugInfo {
    llvm::DICompileUnit *TheCU;
    llvm::DIType *DblTy;
    std::vector<llvm::DIScope *> LexicalBlocks;

    void emitLocation(ExprAST *AST);
    llvm::DIType *getDoubleTy();
};

extern std::unique_ptr<llvm::DIBuilder> DBuilder; // Ensure this is declared as extern
extern DebugInfo KSDbgInfo; // Ensure this is declared as extern

llvm::DISubroutineType *CreateFunctionType(unsigned NumArgs);

#endif // DEBUGINFO_H
