
/**
 * Regina - Software for low-dimensional topology
 * Administrative utilities
 *
 * This program is built and run by "make env".
 * It displays basic information about the underlying OS and architecture.
 */

#include <iostream>
#include <sys/types.h> // for ssize_t

int main() {
    std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
    std::cout << "sizeof(long) = " << sizeof(long) << std::endl;
    std::cout << "sizeof(size_t) = " << sizeof(size_t) << std::endl;
    std::cout << "sizeof(ssize_t) = " << sizeof(ssize_t) << std::endl;
    return 0;
}
