#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable ready;
bool flag = false;
const int N = 500000;

void ping() {
    for (int i = 0; i < N; i++) {
        std::unique_lock<std::mutex> lock(m);
        while(flag) {
            ready.wait(lock);
        }
        std::cout << "ping\n";
        flag = true;
        ready.notify_one();
    }
}

void pong() {
    for (int i = 0; i < N; i++) {
        std::unique_lock<std::mutex> lock(m);
        while(!flag) {
            ready.wait(lock);
        }
        std::cout << "pong\n";
        flag = false;
        ready.notify_one();
    }
}

int main() {
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}