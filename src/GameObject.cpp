#include "GameObject.h"

CGameObject::CGameObject(void)
{
    m_gameState = GS_STOPPED;
    
    this->m_programsList.push_back(new COSProgram());
    this->m_programsList.push_back(new CBrowserProgram());
    
    m_userMood = USER_MOOD_MAX;
    m_runTime = 0;
}

CGameObject::~CGameObject(void)
{
    Reset();
    while(this->m_programsList.size())
    {
        CProgramBase *pProgram = (CProgramBase *)(this->m_programsList.back());
        this->m_programsList.pop_back();
        if (pProgram)
        {
            delete pProgram;
            pProgram = NULL;
        }
    }
    
    
}

void CGameObject::RandomizeMatrix()
{
    srand ( time(NULL) );
    int maxOps = rand() % 100 + 1;
    
    //*
    // test the matrix
    for (int i=0;i<maxOps;i++)
    {
        int rPage = rand() % 20 + 1;
        int rBytes = rand() % 20 + 1;
        int rVal = rand() % 5 + 1;
        int rLenght = rand() % 25 + 1;
        int rStart = rand() % 25 + 1;
        
        // more inserts
        int rInsert = rand() % 10;
        
        if (rInsert)
        {
            m_matrix.insert(rPage,rStart,rVal,rLenght);
        } else {
            // delete
            m_matrix.remove(rPage, rBytes, rVal, rLenght);
        }
    }
}

int CGameObject::GetMemoryLocation(int page, int byte)
{
    return m_matrix.getAt(page, byte);
}

void CGameObject::SetGameState(int state)
{
    if (state == GS_RUNNING)
    {
        m_seconds = time(NULL);
    }
    m_gameState = state;   
}

int CGameObject::GetGameState()
{
    return m_gameState;
}

void CGameObject::Simulate()
{
    if (m_gameState != GS_RUNNING)
    {
        return;
    }
    time_t seconds = time(NULL);
    int secondsFromStart = seconds - m_seconds;
    
    //if (secondsFromStart < 1)
    //{
//        return;
//    }
    
    std::vector<CProgramBase *>::const_iterator iter;
    for (iter = m_programsList.begin(); iter != m_programsList.end(); iter++)
    {
        PROGRAM_OP op = (*iter)->GetOperation(secondsFromStart);
        if (op.act != OP_NONE) 
        {
            OP_QUEUE_ITEM *opi = new OP_QUEUE_ITEM;
                   
            opi->lenght = op.lenght;
            opi->program = (*iter)->GetCode();
                    
            if (op.act == OP_INSERT)
            {
                opi->op = OP_INSERT;
            } else {
                opi->op = OP_REMOVE;
            }
            
            if (m_requestsQueue.size() < OP_MAX_QUEUE_LENGHT)   
            {
                m_requestsQueue.push_back(opi);
            } else {
                // affect the mod
                m_userMood -= (*iter)->GetBadMod();
                if (m_userMood<=0)
                {
                    m_gameState = GS_GAME_OVER;
                }
            }
        }
    }
    m_runTime = time(NULL) - m_seconds;
}

std::vector<OP_QUEUE_ITEM *> CGameObject::GetQueue()
{
    return m_requestsQueue;
}

int CGameObject::GetUserMood()
{
    return m_userMood;
}

int CGameObject::GetRunTime()
{
    return m_runTime;
}

void CGameObject::Reset()
{
    m_gameState = GS_STOPPED;
    
    while(this->m_requestsQueue.size())
    {
        OP_QUEUE_ITEM *item = (OP_QUEUE_ITEM *)(this->m_requestsQueue.back());
        this->m_requestsQueue.pop_back();
        if (item)
        {
            delete item;
            item = NULL;
        }
    }
    
    m_userMood = USER_MOOD_MAX;
    m_seconds = 0;
    m_runTime = 0;
}


OP_QUEUE_ITEM * CGameObject::GetNextOp()
{
    if (m_requestsQueue.size())
    {
        return (*m_requestsQueue.begin());
    }
    return NULL;
}


bool CGameObject::OpFits(int page, int byte)
{
    if (m_requestsQueue.size())
    {
        OP_QUEUE_ITEM *currentOp = (*m_requestsQueue.begin());
        if (currentOp->op == OP_INSERT) 
        {
            if (m_matrix.fits(page,byte,currentOp->lenght))
            {
                return true;
            }
        } else {
            if (m_matrix.canRemove(page,byte,currentOp->program,currentOp->lenght))
            {
                return true;
            }
        }
    }
    
    return false;
}


bool CGameObject::OpApply(int page, int byte)
{
    if (m_requestsQueue.size())
    {
        OP_QUEUE_ITEM *currentOp = (*m_requestsQueue.begin());
        if (currentOp->op == OP_INSERT) 
        {
            if (m_matrix.insert(page,byte,currentOp->program,currentOp->lenght))
            {
                ModHappyUser(currentOp->program);
                
                // remove from queue
                std::vector<OP_QUEUE_ITEM *>::iterator iter = m_requestsQueue.begin();
                m_requestsQueue.erase(iter);
                delete currentOp;
                currentOp = NULL;
                return true;
            }
        } else {
            if (m_matrix.remove(page,byte,currentOp->program,currentOp->lenght))
            {
                ModHappyUser(currentOp->program);
                
                // remove from queue
                std::vector<OP_QUEUE_ITEM *>::iterator iter = m_requestsQueue.begin();
                m_requestsQueue.erase(iter);
                delete currentOp;
                currentOp = NULL;
                return true;
            }
        }
    }
    
    return false;
}

void CGameObject::ModHappyUser(int pcode)
{
    for (std::vector<CProgramBase *>::const_iterator iter = m_programsList.begin(); iter != m_programsList.end(); iter++)
    {
        CProgramBase *pProgram = (*iter);
        if (pProgram->GetCode() == pcode)
        {
            m_userMood += pProgram->GetGoodMod();
            if (m_userMood > USER_MOOD_MAX) 
            {
                m_userMood = USER_MOOD_MAX;
            }
            break;
        }
    }
}
