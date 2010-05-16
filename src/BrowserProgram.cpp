#include "BrowserProgram.h"

CBrowserProgram::CBrowserProgram(void)
{
    m_goodMod = 1;
    m_badMod = 2;
    m_code = 2;
    m_lastExec = -1;
}

CBrowserProgram::~CBrowserProgram(void)
{
}

PROGRAM_OP CBrowserProgram::GetOperation(int seconds)
{
    // between 1 and max
    PROGRAM_OP ret;
    srand(time(NULL));
    
    if (m_lastExec == -1)
    {
        seconds = 0;
    }
    
    int doit = rand() % 3; // shoul call this "notdoit"
    if (m_lastExec != seconds && doit==1)
    {
        int x = rand()%10;
        if (x) {
            ret.act = OP_INSERT;
            ret.lenght = rand()%OP_MAX_REQUEST_LENGHT+1;
            m_allocationList.push_back(ret.lenght);
        } else {
            ret = GetRemoveOp();
            if (ret.act != OP_REMOVE)
            {
                // skip this one
                ret.act = OP_NONE;
            }
        }
        m_lastExec = seconds;
    } else {
        ret.act = OP_NONE;
    }
    
    return ret;
}