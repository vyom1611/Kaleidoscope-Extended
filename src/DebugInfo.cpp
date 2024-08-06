#include "../include/DebugInfo.h"
#include "llvm/IR/IRBuilder.h"

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;

std::unique_ptr<llvm::DIBuilder> DBuilder; // Ensure this is defined here
DebugInfo KSDbgInfo; // Ensure this is defined here

llvm::DIType *DebugInfo::getDoubleTy() {
    if (DblTy)
        return DblTy;
    DblTy = DBuilder->createBasicType("double", 64, llvm::dwarf::DW_ATE_float);
    return DblTy;
}

void DebugInfo::emitLocation(ExprAST *AST) {
    if (!AST)
        return Builder.SetCurrentDebugLocation(llvm::DebugLoc());
    llvm::DIScope *Scope;
    if (LexicalBlocks.empty())
        Scope = TheCU;
    else
        Scope = LexicalBlocks.back();
    Builder.SetCurrentDebugLocation(llvm::DILocation::get(
        Scope->getContext(), AST->getLine(), AST->getCol(), Scope));
}

llvm::DISubroutineType *CreateFunctionType(unsigned NumArgs) {
    llvm::SmallVector<llvm::Metadata *, 8> EltTys;
    llvm::DIType *DblTy = KSDbgInfo.getDoubleTy();
    EltTys.push_back(DblTy);
    for (unsigned i = 0; i != NumArgs; ++i)
        EltTys.push_back(DblTy);
    return DBuilder->createSubroutineType(DBuilder->getOrCreateTypeArray(EltTys));
}
