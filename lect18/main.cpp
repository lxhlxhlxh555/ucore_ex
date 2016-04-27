#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <vector>
 
using namespace std;

// 参数调整
int reader_num = 10; // 产生读者的总数
int seat_size = 5; // 总座位数
int read_max_time = 200; // 最大读书时间
int wait_max_time = 100; // 最大等座位时间
auto reader_gen_time = chrono::milliseconds(20); // 读者生成速度

// 全局变量
mutex m;
condition_variable cv;
int seat = seat_size; // 当前座位数
auto start = chrono::system_clock::now();

double getTime() {
    return chrono::duration<double>(std::chrono::system_clock::now()-start).count()*1000;
}

// 读者
class Reader {
public:
    int id, wait_time, read_time;
    Reader(int id, int wait_time, int read_time)
        : id(id), wait_time(wait_time), read_time(read_time) {
        }
    void action() { // 读者行为
        unique_lock<mutex> lk(m); // monitors begin
        printf("%d\t%d\t%.2lf\twait\n",id, seat, getTime());

        // 等待事件：座位数有一个或以上
        if (!cv.wait_for(lk, chrono::milliseconds(wait_time), [&]{ return seat >= 1;})) {
            printf("%d\t%d\t%.2lf\ttimeout\twait time: %d\n",id,seat, getTime(),wait_time);
            return;
        }
        seat--;
        printf("%d\t%d\t%.2lf\tenter\n",id, seat, getTime());

        // 等待事件：阅读时间达到read_time以上
        auto temp = chrono::system_clock::now(); // 开始阅读时间
        cv.wait_for(lk, chrono::milliseconds(read_time), [&]{
            auto end = chrono::system_clock::now();
            chrono::duration<double> diff = end-temp; // 时间差
            return diff.count() > read_time;
        });
        seat++;
        printf("%d\t%d\t%.2lf\tleave\tread_time: %d\n",id,seat,getTime(), read_time);

        lk.unlock(); // monitors end
        cv.notify_one();
    }
};

// 读者产生器
void reader_generator(int size) {
    vector<thread*> ths;
    printf("id\tseat\ttime\tstate\tcomment\n");
    for (int i = 0; i < size; ++i)
    {
        int read_time = random()%read_max_time;
        int wait_time = random()%wait_max_time;
        thread* th = new thread(&Reader::action,Reader(i,wait_time,read_time));
        ths.push_back(th);
        this_thread::sleep_for(reader_gen_time);
    }
    for (int i = 0; i < size; ++i)
    {
        ths[i]->join();
    }
}
 
int main(int argc, char** argv)
{
    if (argc >= 6) {
        printf("./main reader_num seat_size read_max_time wait_max_time reader_gen_time\n");
        reader_num = atoi(argv[1]);
        seat_size = atoi(argv[2]);
        read_max_time = atoi(argv[3]);
        wait_max_time = atoi(argv[4]);
        reader_gen_time = chrono::milliseconds(atoi(argv[5]));
    }

    start = chrono::system_clock::now();
    reader_generator(reader_num);
    return 0;
}