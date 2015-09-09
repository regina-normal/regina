
#include <thread>

void run0() {
}

void run1(int) {
}

int main() {
    std::thread t0(run0);
    std::thread t1(run1, 2);
    t0.join();
    t1.join();
    return 0;
}

