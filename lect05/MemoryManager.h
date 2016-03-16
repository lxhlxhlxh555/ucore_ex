//
// Created by lxhlxhlxh555 on 16/3/7.
//

#ifndef MM_MEMORYMANAGER_H
#define MM_MEMORYMANAGER_H

#include <list>
#include "MemBlock.h"

// 抽象类:内存管理器
class MemoryManager {
public:
    MemoryManager(std::string name, int size) {
        mem = (char *) malloc(sizeof(char) * size);
        this->name = name;
        this->size = size;
    }

    ~MemoryManager() {
        free(mem);
    }

    std::string& getName() {
        return name;
    }

    virtual char* myAlloc(int size) = 0;
    virtual void myFree(char *ptr) = 0;
    virtual int randomErase() = 0;
    virtual void defrag() = 0;

    virtual int getFreeMemorySize() = 0;
    virtual int getFreeBlockSize() = 0;
    virtual int getUsedBlockSize() = 0;

protected:
    std::string name;
    char* mem;
    int size;
};

#endif //MM_MEMORYMANAGER_H
