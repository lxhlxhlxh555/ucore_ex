//
// Created by lxhlxhlxh555 on 16/3/7.
//

#ifndef MM_BESTFIT_H
#define MM_BESTFIT_H

#include <set>
#include "MemoryManager.h"

template<class Compare>
class SizeFit : public MemoryManager {
public:
    SizeFit(std::string name, int size);

    ~SizeFit();

    char *myAlloc(int size);

    void myFree(char *ptr);

    int randomErase();

    void defrag();

    int getFreeMemorySize();

    int getFreeBlockSize();

    int getUsedBlockSize();

    template<class T>
    friend std::ostream &operator<<(std::ostream &out, SizeFit<T> &mm);

private:
    std::multiset<MemBlock, Compare> mems;
};


template<class Compare>
SizeFit<Compare>::SizeFit(std::string name, int size) : MemoryManager(name, size) {
    mems.insert(MemBlock(mem, size));
}

template<class Compare>
SizeFit<Compare>::~SizeFit() {

}

template<class Compare>
char *SizeFit<Compare>::myAlloc(int size) {
    if (size <= 0) {
        return nullptr;
    }

    auto block = mems.begin();
    // 找第一个足够大的块
    for (; block != mems.end(); ++block) {
        if (block->getSize() > size && !block->isUsed()) {
            break;
        }
    }

    if (block != mems.end()) { // 找到
        MemBlock oldBlock = *block;
        MemBlock newBlock = oldBlock.split(size);
        newBlock.setUsed();
        mems.erase(block);
        mems.insert(oldBlock);
        mems.insert(newBlock);
        this->size -= size;
        return newBlock.getPos();
    } else {
        return nullptr;
    }
}

template<class Compare>
void SizeFit<Compare>::myFree(char *ptr) {
    auto block = mems.begin();
    for (; block != mems.end(); ++block) {
        if ((block->getPos() == ptr) && block->isUsed()) {
            break;
        }
    }

    if (block == mems.end()) { // 没有找到
        return;
    } else {
        this->size += block->getSize();
    }
    MemBlock cur = *block;
    mems.erase(block);
    cur.setUnUsed();
    block = mems.insert(cur);

    for (auto pre = mems.begin(); pre != mems.end(); ++pre) {
        if (pre->isContinuous(cur) && !pre->isUsed()) {
            MemBlock tempBlock = *pre;
            tempBlock.addSize(cur.getSize());
            mems.erase(block);
            mems.erase(pre);
            block = mems.insert(tempBlock);
            cur = *block;
            break;
        }
    }

    for (auto next = mems.begin(); next != mems.end(); ++next) {
        if (cur.isContinuous(*next) && !next->isUsed()) {
            cur.addSize(next->getSize());
            mems.erase(block);
            mems.erase(next);
            mems.insert(cur);
            break;
        }
    }
}

template<class Compare>
int SizeFit<Compare>::randomErase() {
    int usedSize = getUsedBlockSize();
    if (usedSize == 0) return 0;
    char *ptr = 0;
    int count = 0;
    int no = (int) (random() % usedSize);
    int size;
    for (auto block = mems.begin(); block != mems.end(); ++block) {
        if (block->isUsed()) {
            count++;
        }
        if (count == no) {
            ptr = block->getPos();
            size = block->getSize();
        }
    }
    myFree(ptr);
    return size;
}

template<class Compare>
void SizeFit<Compare>::defrag() {
    auto tempMems = mems;
    mems.clear();
    size = 0;
    char *pos = mem;
    for (auto block : tempMems) {
        if (block.isUsed()) {
            block.move(pos);
            pos += block.getSize();
            mems.insert(block);
        } else {
            size += block.getSize();
        }
    }
    mems.insert(MemBlock(pos, size));
}

template<class Compare>
int SizeFit<Compare>::getFreeMemorySize() {
    return mems.size();
}

template<class Compare>
int SizeFit<Compare>::getFreeBlockSize() {
    int count = 0;
    for (auto block : mems) {
        if (!block.isUsed()) {
            ++count;
        }
    }
    return count;
}

template<class Compare>
int SizeFit<Compare>::getUsedBlockSize() {
    int count = 0;
    for (auto block : mems) {
        if (block.isUsed()) {
            ++count;
        }
    }
    return count;
}

template<class Compare>
std::ostream &operator<<(std::ostream &out, SizeFit<Compare> &mm) {
    out << "free size: " << mm.size << '\n';
    for (auto &block : mm.mems) {
        out << block << "\n";
    }
    return out;
}

#endif //MM_BESTFIT_H
