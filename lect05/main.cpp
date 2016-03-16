#include <iostream>
#include <vector>
#include "MemBlock.h"
#include "FirstFit.h"
#include "SizeFit.h"

using namespace std;

// 测试目标
// 在有足够内存的情况下,观察三种分配策略的碎片数

class Sequence {
public:
    Sequence(unsigned int size) {
        this->size = size;
    }

    unsigned int getNum(unsigned int min, unsigned int max) {
        if (max < min) return 0;
        unsigned int range = max - min;
        if (size < max) return 0;
        unsigned int ret = (unsigned int) (random() % range + min);
        size -= ret;
        return ret;
    }

    void inc(unsigned int num) {
        size += num;
    }

private:
    unsigned int size;
};

int main(int argc, char **argv) {

//    if (argc < 4) {
//        cout << "please input parameters" << endl;
//        cout << "./mm loopSize memSize min max" << endl;
//        return -1;
//    }
//    int loopSize = atoi(argv[1]);
//    int memSize = atoi(argv[2]);
//    int min = atoi(argv[3]);
//    int max = atoi(argv[4]);

    int loopSize = 200;
    int memSize = 1000;
    int min = 10;
    int max = 100;

    FirstFit first("first", memSize);
    SizeFit<Compare::BestFit> best("best", memSize);
    SizeFit<Compare::WorstFit> worst("worst", memSize);

    vector<MemoryManager *> mms;
    mms.push_back(&first);
    mms.push_back(&best);
    mms.push_back(&worst);

    srandom(time(0));
    cout << "mem size: " << memSize << endl;
    cout << "block range:[" << min << "," << max << "]" << endl;
    for (auto &mm : mms) {
        int defragCount = 0;
        int fragCount = 0;
        Sequence sequence(memSize);

        for (int i = 0; i < 10; ++i) {
            unsigned int num = sequence.getNum(30, 50);
            mm->myAlloc(num);
        }

        for (int i = 0; i < loopSize; ++i) {
            unsigned int num = sequence.getNum(min, max);
            char *ptr = mm->myAlloc(num);
            if (ptr == nullptr) {
                fragCount += mm->getFreeBlockSize();
                defragCount++;
                int temp = mm->randomErase();
                sequence.inc(temp);

//                mm->defrag(); // 碎片整理
            }
            if (random() % 2 == 0) {
                int temp = mm->randomErase();
                sequence.inc(temp);
            }
        }

        cout << mm->getName() << ":"
             << "fragCount:" << fragCount
             << ", count:" << defragCount << endl;
    }

    return 0;
}