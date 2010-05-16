#include "ProgramBase.h"

CProgramBase::CProgramBase(void)
{
    m_goodMod = 0;
    m_badMod = 0;
    m_code = 0;
}

CProgramBase::~CProgramBase(void)
{
}


PROGRAM_OP CProgramBase::GetRemoveOp()
{
    PROGRAM_OP ret;
    
    if (m_allocationList.empty())
    {
        ret.act = OP_NONE;
    } else {
        // get a random op from the alloc list
        srand(time(NULL));
        int idx = rand()%(m_allocationList.size());
        ret.act = OP_REMOVE;
        std::vector<int>::iterator iter;
        iter = m_allocationList.begin()+idx;
        ret.lenght = (*iter);
        m_allocationList.erase(iter);
    }
    
    return ret;
}