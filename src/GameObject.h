#pragma once
#include "stdafx.h"
#include <vector>
#include "MemoryMatrix.h"

#include "ProgramBase.h"
#include "OSProgram.h"
#include "BrowserProgram.h"

enum 
{
    GS_STOPPED = 1,
    GS_RUNNING,
    GS_PAUSED,
    GS_GAME_OVER
};

typedef struct _OP_QUEUE_ITEM
{
    int op;
    int program;
    int lenght;
} OP_QUEUE_ITEM;

class CGameObject
{
public:
    CGameObject(void);
    ~CGameObject(void);
    
    // game state
    int GetGameState();
    void SetGameState(int state);
    
    // game data    
    int GetUserMood();
    int GetMemoryLocation(int page, int byte);
    std::vector<OP_QUEUE_ITEM *> GetQueue();
    OP_QUEUE_ITEM * GetNextOp();
    
    bool OpFits(int page, int byte);
    bool OpApply(int page, int byte);
    
    void RandomizeMatrix();
    void Simulate();
    int GetRunTime();
    
    void Reset();
    time_t GetGameStartTime();
private:
    int m_gameState;
    CMemoryMatrix m_matrix;
    std::vector<CProgramBase *> m_programsList;
    std::vector<OP_QUEUE_ITEM *> m_requestsQueue;
    
    void ModHappyUser(int pcode);
    
    
    static const int m_queueMax = 5;
    int m_userMood;
    time_t m_seconds;
    int m_runTime;
};
