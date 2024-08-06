#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include <memory>
#include <map>

extern std::map<char, int> BinopPrecedence; // Declare as extern
extern int CurTok;

std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<PrototypeAST> ParsePrototype();
std::unique_ptr<FunctionAST> ParseDefinition();
std::unique_ptr<FunctionAST> ParseTopLevelExpr();
std::unique_ptr<PrototypeAST> ParseExtern();

#endif // PARSER_H
