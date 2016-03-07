实验结果
    无碎片整理
        mem size: 1000
        block range:[10,100]
        first:fragCount:1949, count:115
        best:fragCount:550, count:69
        worst:fragCount:831, count:73

    有碎片整理
        mem size: 1000
        block range:[10,100]
        first:fragCount:136, count:57
        best:fragCount:131, count:56
        worst:fragCount:140, count:57

    参数说明
        mem size 内存大小
        block range 申请内存块大小范围
        count 遇到内存不足的次数
        fragCount 遇到内存不足时碎片的个数

实验结论
    在无碎片整理的情况下
        优先匹配遇到内存不足的情况多，产生碎片多。
        表现最好是最佳匹配，其次是最差匹配。
    在有碎片整理的情况下
        表现差不多

实验框架说明
    类
        MemoryManager 内存管理器
            抽象类，声明alloc,free等虚函数
        FirstFit 优先匹配
            实现MemoryManager
        SizeFit<Compare::BestFit> 最佳匹配
            实现MemoryManager
        SizeFit<Compare::WorstFit> 最差匹配
            实现MemoryManager
        MemBlock 内存块
        Sequence 序列产生
            产生有一定的随机数，用于申请内存
    实验主体
        1、每种内存管理策略的内存大小相等。
        2、对每种内存管理策略，重复以下过程：
            1、随机申请内存，大小为range[min,max]
            2、如果内存不足，则随机释放一个内存块，[碎片整理]，同时统计数据
            3、有50%的概率随机释放内存块
        3、输出统计数据

