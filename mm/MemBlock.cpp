
#include "MemBlock.h"

MemBlock::MemBlock(char *pos, int size) {
    this->pos = pos;
    this->size = size;
    this->used = false;
}

MemBlock::MemBlock() {
    pos = 0;
    size = 0;
    this->used = false;
}

MemBlock::~MemBlock() {

}

MemBlock MemBlock::split(int size) {
    if (size > this->size) {
        return MemBlock();
    } else {
        char *temp = pos;
        this->pos += size;
        this->size -= size;
        return MemBlock(temp, size);
    }
}

bool MemBlock::isContinuous(const MemBlock& block) const {
    return pos + size == block.pos;
}

void MemBlock::addSize(int size) {
    this->size += size;
}

void MemBlock::move(char *pos) {
    this->pos = pos;
}

std::ostream &operator<<(std::ostream &out,const MemBlock &block) {
    char buf[32];
    sprintf(buf, "used:%d p:%llx size:%d", block.used, (long long int) block.pos, block.size);
    out << buf;
    return out;
}

