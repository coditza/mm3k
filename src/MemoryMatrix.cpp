#include "MemoryMatrix.h"



CMemoryMatrix::CMemoryMatrix(void)
{
    m_activePages = 20;
    
    for (int pages=0; pages < MAX_PAGES; pages++)
    {
        for (int bytes = 0; bytes < MAX_BITS; bytes++) 
        {
            m_memoryMatrix[pages][bytes] = EMPTY_SPACE;
        }
    }
}

CMemoryMatrix::~CMemoryMatrix(void)
{
}

bool CMemoryMatrix::fits(int page, int start, int length)
{
    if (page >= MAX_BITS || page < 0 || page > m_activePages)
    {
        return false;
    }
    
    int length_found = 0;
    for (int i=start;i<MAX_BITS && i<start+length; i++)
    {
        if (m_memoryMatrix[page][i] != EMPTY_SPACE) {
            length_found = 0;
        } else if (m_memoryMatrix[page][i]  == EMPTY_SPACE) {
            length_found++;
            if (length_found >= length)
            {
                // found enough space
                return true;
            }
        }
    }
    
    return false;
}

bool CMemoryMatrix::insert(int page, int start, int val, int length)
{
    if (page >= MAX_BITS || page < 0 || page > m_activePages)
    {
        return false;
    }
    
    int length_found = 0;
    int last_start = start;
    int i = 0;
    for (int i=start;i<MAX_BITS && i<start+length; i++)
    {
        if (m_memoryMatrix[page][i] != EMPTY_SPACE) {
            length_found = 0;
            last_start = i;
        } else if (m_memoryMatrix[page][i] == EMPTY_SPACE) {
            length_found++;
            if (length_found == length)
            {
                // found enough space
                break;
            }
        }
    }

    if (length_found == length)
    {
        for (i=last_start;i<last_start+length;i++)
        {
            m_memoryMatrix[page][i] = val;
        }
        return true;
    }

    return false;
}

bool CMemoryMatrix::canRemove(int page, int start, int val, int length)
{
    if (page >= MAX_BITS || page < 0 || page > m_activePages)
    {
        return false;
    }
    
    for (int i=start;i<start+length;i++)
    {
        if (m_memoryMatrix[page][i] != val)
        {
            return false;
        }
    }
    return true;
}

bool CMemoryMatrix::remove(int page, int start, int val, int length)
{
    if (page >= MAX_BITS || page < 0 || page > m_activePages)
    {
        return false;
    }
    
    // go to start position and see if we can remove so many items
    for (int i=start;i<start+length;i++)
    {
        if (m_memoryMatrix[page][i] != val)
        {
            return false;
        } else {
            // remove byte
            m_memoryMatrix[page][i] = EMPTY_SPACE;
        }
    }   
    return true;
}

int CMemoryMatrix::getAt(int page, int byte)
{
    return m_memoryMatrix[page][byte];
}