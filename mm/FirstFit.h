
#ifndef MY_PMM_MM_FIRST_H
#define MY_PMM_MM_FIRST_H

#include <stdlib.h>
#include <list>
#include "MemBlock.h"
#include "MemoryManager.h"

class FirstFit : public MemoryManager {
public:
    FirstFit(std::string name, int size);
    ~FirstFit();
    char*myAlloc(int size);
    void myFree(char *ptr);
    int randomErase();
    void defrag();
    int getFreeMemorySize();
    int getFreeBlockSize();
    int getUsedBlockSize();
    friend std::ostream&operator << (std::ostream& out, FirstFit& mm);
private:
    std::list<MemBlock> mems;
};


#endif //MY_PMM_MM_FIRST_H
