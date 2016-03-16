#include "FirstFit.h"

using namespace std;

FirstFit::FirstFit(string name, int size) : MemoryManager(name,size) {
    mems.push_back(MemBlock(mem, size));
}

FirstFit::~FirstFit() {

}

char *FirstFit::myAlloc(int size) {
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
        auto newBlock = mems.insert(block, block->split(size));
        this->size -= size;
        newBlock->setUsed();
        return newBlock->getPos();
    } else {
        return nullptr;
    }

}

void FirstFit::myFree(char *ptr) {
    auto block = mems.begin();
    for (; block != mems.end(); ++block) {
        if ((block->getPos() == ptr) && block->isUsed()) {
            break;
        }
    }

    if (block != mems.end()) { // 找到要释放的块
        block->setUnUsed();
        this->size += block->getSize();

        // 前一个内存块未使用,且二者相连
        auto pre = block;
        if (block != mems.begin()) {
            pre--;
            if (!pre->isUsed() && pre->isContinuous(*block)) {
                pre->addSize(block->getSize());
                mems.erase(block);
                block = pre;
            }
        }

        // 后一个内存块未使用,且二者相连
        auto next = block;
        next++;
        if (next != mems.end()) {
            if (!next->isUsed() && block->isContinuous(*next)) {
                block->addSize(next->getSize());
                mems.erase(next);
            }
        }
    } else { // 未找到需要释放的块

    }
}

int FirstFit::randomErase() {
    int usedSize = getUsedBlockSize();
    if (usedSize == 0) return 0;
    char* ptr = 0;
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

void FirstFit::defrag() {
    auto tempMems = mems;
    mems.clear();
    size = 0;
    char* pos = mem;
    for (auto block : tempMems) {
        if (block.isUsed()) {
            block.move(pos);
            pos += block.getSize();
            mems.push_back(block);
        } else {
            size += block.getSize();
        }
    }
    mems.push_back(MemBlock(pos,size));
}

int FirstFit::getFreeMemorySize() {
    return mems.size();
}

int FirstFit::getFreeBlockSize() {
    int count = 0;
    for (auto block : mems) {
        if (!block.isUsed()) {
            ++count;
        }
    }
    return count;
}

int FirstFit::getUsedBlockSize() {
    int count = 0;
    for (auto block : mems) {
        if (block.isUsed()) {
            ++count;
        }
    }
    return count;
}

std::ostream &operator<<(std::ostream &out, FirstFit &mm) {
    out << "free size: " << mm.size << '\n';
    for (auto &block : mm.mems) {
        out << block << "\n";
    }
    return out;
}

