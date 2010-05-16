#include "OSProgram.h"

COSProgram::COSProgram(void)
{
    m_goodMod = 1;
    m_badMod = 1;
    m_code = 1;
    m_lastExec = -1;
}

COSProgram::~COSProgram(void)
{
}

PROGRAM_OP COSProgram::GetOperation(int seconds)
{
    // no deallocation (so, you are running windows :D), between 3 and max units at once
    PROGRAM_OP ret;
    srand(time(NULL));
    
    if (m_lastExec == -1)
    {
        seconds = 0;
    }
    
    int doit = rand() % 10+1;
    if (m_lastExec != seconds && seconds%30 == 0)
    {
        ret.act = OP_INSERT;
        ret.lenght = 0;
        while (ret.lenght < 3)
        {
            ret.lenght = rand()%OP_MAX_REQUEST_LENGHT+1;
        }
        m_allocationList.push_back(ret.lenght);
        m_lastExec = seconds;
    } else {
        ret.act = OP_NONE;
    }
    
    
    return ret;
}