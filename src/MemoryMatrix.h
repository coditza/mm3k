#pragma once
#include "stdafx.h"
#include <vector>

typedef int tMemoryMatrix[20][25];

class CMemoryMatrix
{
public:
    CMemoryMatrix(void);
    ~CMemoryMatrix(void);
    
    bool fits(int page, int start, int length);
    bool insert(int page, int start, int val, int length);
    bool canRemove(int page, int start, int val, int length);
    bool remove(int page, int start, int val, int length);
    
    int getAt(int page, int byte);
private:
    tMemoryMatrix m_memoryMatrix;
    int m_activePages;
};
