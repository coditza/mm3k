#pragma once
#include "stdafx.h"
#include <vector>

typedef struct _PROGRAM_OP
{
    int act;
    int lenght;
} PROGRAM_OP;

class CProgramBase
{
public:
    CProgramBase(void);
    ~CProgramBase(void);
    
    virtual int GetGoodMod() { return m_goodMod; };
    virtual int GetBadMod() { return m_badMod; };
    virtual int GetCode() { return m_code; }
    virtual PROGRAM_OP GetOperation(int seconds) = 0;
    
protected:
    PROGRAM_OP GetRemoveOp();
    
protected:
    int m_goodMod;
    int m_badMod;
    int m_code;
    time_t m_lastExec;
    std::vector<int> m_allocationList;
};
