# report

### 1. 阅览室问题

有一个阅览室，共有100个座位。
读者进入时必须先在一张登记表上登记，该表为每一个座位列一表目，包括座号和读者姓名等，读者离开时要消掉登记的信息。

试问：

	(1)为描述读者的动作，应编写几个程序，设置几个进程？
	(2)试用PV操作描述各个进程之间的同步互斥关系。

### 2. 问题建模和实现

问题存在不清晰的地方，例如读者拿到座位后，何时离开？读者等待时间多长？如果不手动sleep“制造”耗时操作，那么仅100个座位的规模需要大量的读者操作才能体现多线程PV问题。因此，我假设读者有愿意等待时间、占有座位时间。

	ReaderGenerator 读者产生器
		随机一个时间，产生一个读者，每个读者用一个线程模拟
	Reader 读者
		id 读者唯一标识。读者产生器的counter分配。
		wait_time 如果没有座位，愿意等待的时间，-1表示不等待
		read_time 占用座位的时间

为了具备更多可操作性，我设置若干变量供调整。

	int reader_num = 10; // 产生读者的总数
	int seat_size = 5; // 总座位数
	int read_max_time = 200; // 最大读书时间
	int wait_max_time = 100; // 最大等座位时间
	int reader_gen_time = 20; // 读者生成速度
	
代码用c++11的线程支持库(Thread support library)实现，不依赖Linux/Unix，可跨Linux/Unix、Windows、OS X运行。

### 3. 实验结果及分析

1. 基本实验

	默认参数，输出结果表格整理如下：
	
	|id|seat|time|state|comment|
	|---|---|---|---|---|
	|0|5|0.14|wait|
	|0|4|0.18|enter|
	|1|4|20.70|wait|
	|1|3|20.74|enter|
	|2|3|40.72|wait|
	|2|2|40.75|enter|
	|3|2|64.07|wait|
	|3|1|64.14|enter|
	|4|1|87.26|wait|
	|4|0|87.29|enter|
	|5|0|107.51|wait|
	|6|0|127.96|wait|
	|5|0|137.40|timeout|wait time: 27|
	|4|1|137.45|leave|read_time: 49|
	|7|1|152.81|wait|
	|7|0|152.89|enter|
	|8|0|177.16|wait|
	|0|1|183.83|leave|read_time: 183|
	|6|0|187.50|enter|
	|9|0|197.57|wait|
	|1|1|197.80|leave|read_time: 177|
	|8|0|204.38|enter|
	|2|1|238.11|leave|read_time: 193|
	|9|0|238.18|enter|
	|3|1|253.19|leave|read_time: 186|
	|6|2|282.55|leave|read_time: 90|
	|7|3|320.96|leave|read_time: 163|
	|8|4|346.53|leave|read_time: 140|
	|9|5|410.82|leave|read_time: 172|
	
	通过数据可基本验证程序是正确的。
	
	例如id为5的读者，在137.40时等待超时（耗时27），他在107.51开始等待，结果正确。
	
	例如id为4的读者，在137.45时阅读完成并离开（耗时49），他在87.29进入，结果正确。

2. 调整参数

	可以调整参数影响结果，例如增大最大读书时间，那么应该有更多的读者因为等待时间长而离开。
	
	调整最大读书时间，从200增加到300：
		
		// int read_max_time = 300; 
		
		$ ./main 10 5 300 100 20
	
	输出结果为：
	
	|id|seat|time|state|comment|
	|---|---|---|---|---|
	|0|5|0.28|wait|
	|0|4|0.30|enter|
	|1|4|24.17|wait|
	|1|3|24.20|enter|
	|2|3|48.16|wait|
	|2|2|48.19|enter|
	|3|2|69.24|wait|
	|3|1|69.28|enter|
	|4|1|90.52|wait|
	|4|0|90.71|enter|
	|5|0|111.52|wait|
	|6|0|131.76|wait|
	|5|0|138.72|timeout|wait time: 27|
	|7|0|153.02|wait|
	|8|0|178.15|wait|
	|7|0|181.50|timeout|wait time: 26|
	|6|0|190.84|timeout|wait time: 59|
	|9|0|199.35|wait|
	|1|1|201.33|leave|read_time: 177|
	|8|0|204.56|enter|
	|9|0|238.36|timeout|wait time: 36|
	|0|1|284.27|leave|read_time: 283|
	|4|2|340.08|leave|read_time: 249|
	|2|3|346.26|leave|read_time: 293|
	|3|4|359.74|leave|read_time: 286|
	|8|5|449.54|leave|read_time: 240|
	
	timeout的个数明显增加了。
