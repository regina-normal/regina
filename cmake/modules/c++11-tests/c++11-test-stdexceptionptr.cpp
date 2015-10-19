#include <string>
#include <exception>
#include <stdexcept>

int main() {
    std::exception_ptr p;
    try {
        std::string().at(1);
    } catch(...) {
        p = std::current_exception();
        return 0;
    }
    return 1;
}
