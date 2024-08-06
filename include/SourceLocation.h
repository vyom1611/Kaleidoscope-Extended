#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H

struct SourceLocation {
    int Line;
    int Col;
};

extern SourceLocation CurLoc;
extern SourceLocation LexLoc;

#endif // SOURCELOCATION_H
