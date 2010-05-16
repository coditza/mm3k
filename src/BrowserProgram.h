#pragma once
#include "ProgramBase.h"

class CBrowserProgram :
    public CProgramBase
{
public:
    CBrowserProgram(void);
    ~CBrowserProgram(void);
    PROGRAM_OP GetOperation(int milliseconds);
};
