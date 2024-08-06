#include "../include/Lexer.h"
#include "../include/SourceLocation.h"
#include <cctype>
#include <cstdio>

// Definition of external variables
SourceLocation CurLoc;
SourceLocation LexLoc = {1, 0};

std::string IdentifierStr;
double NumVal;

static int advance() {
    int LastChar = getchar();
    if (LastChar == '\n' || LastChar == '\r') {
        LexLoc.Line++;
        LexLoc.Col = 0;
    } else {
        LexLoc.Col++;
    }
    return LastChar;
}

int gettok() {
    static int LastChar = ' ';
    while (isspace(LastChar))
        LastChar = advance();

    CurLoc = LexLoc;

    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = advance())))
            IdentifierStr += LastChar;
        if (IdentifierStr == "def") return tok_def;
        if (IdentifierStr == "extern") return tok_extern;
        if (IdentifierStr == "if") return tok_if;
        if (IdentifierStr == "then") return tok_then;
        if (IdentifierStr == "else") return tok_else;
        if (IdentifierStr == "for") return tok_for;
        if (IdentifierStr == "in") return tok_in;
        if (IdentifierStr == "binary") return tok_binary;
        if (IdentifierStr == "unary") return tok_unary;
        if (IdentifierStr == "var") return tok_var;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = advance();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#') {
        do
            LastChar = advance();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if (LastChar != EOF)
            return gettok();
    }

    if (LastChar == EOF)
        return tok_eof;

    int ThisChar = LastChar;
    LastChar = advance();
    return ThisChar;
}

std::string getTokName(int Tok) {
    switch (Tok) {
        case tok_eof: return "eof";
        case tok_def: return "def";
        case tok_extern: return "extern";
        case tok_identifier: return "identifier";
        case tok_number: return "number";
        case tok_if: return "if";
        case tok_then: return "then";
        case tok_else: return "else";
        case tok_for: return "for";
        case tok_in: return "in";
        case tok_binary: return "binary";
        case tok_unary: return "unary";
        case tok_var: return "var";
    }
    return std::string(1, (char)Tok);
}

