#pragma once
#include "ProgramBase.h"

class COSProgram :
    public CProgramBase
{
public:
    COSProgram(void);
    ~COSProgram(void);
    PROGRAM_OP GetOperation(int seconds);
};
