#ifndef MY_PMM_MEMBLOCK_H
#define MY_PMM_MEMBLOCK_H

#include <iostream>

// 内存块
class MemBlock {
public:
    MemBlock();

    MemBlock(char *pos, int size);

    ~MemBlock();

    MemBlock split(int size);

    bool isContinuous(const MemBlock& block) const;

    void addSize(int size);

    void move(char* pos);

    char *getPos() const {
        return pos;
    }

    int getSize() const {
        return size;
    }

    bool isUsed() const {
        return used;
    }

    void setUsed() {
        used = true;
    }

    void setUnUsed() {
        used = false;
    }

    bool bestFit(MemBlock &b1, MemBlock &b2) const {
        return b1.size < b2.size;
    }

    bool worstFit(MemBlock &b1, MemBlock &b2) const {
        return b1.size > b2.size;
    }

    friend std::ostream &operator<<(std::ostream &out, const MemBlock &block);

private:
    char *pos;
    int size;
    bool used;
};

namespace Compare {

    class BestFit {
    public:
        bool operator()(const MemBlock &b1, const MemBlock &b2) const {
            if (b1.getSize() < b2.getSize()) {
                return true;
            } else if (b2.getSize() < b1.getSize()) {
                return false;
            } else {
                return b1.getPos() < b2.getPos();
            }
        }
    };

    class WorstFit {
    public:
        bool operator()(const MemBlock &b1, const MemBlock &b2) const {
            if (b2.getSize() < b1.getSize()) {
                return true;
            } else if (b1.getSize() < b2.getSize()) {
                return false;
            } else {
                return b1.getPos() < b2.getPos();
            }
        }
    };
}

#endif //MY_PMM_MEMBLOCK_H
