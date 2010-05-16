#pragma once
#include "programbase.h"

class CBrowserProgram :
    public CProgramBase
{
public:
    CBrowserProgram(void);
    ~CBrowserProgram(void);
    PROGRAM_OP GetOperation(int milliseconds);
};
