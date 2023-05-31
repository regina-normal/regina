#include <cstddef>
#include <getopt.h>

static struct option longopts[] = {
    { "foo", no_argument, nullptr, 'f' },
    { "bar", required_argument, nullptr, 'b' },
    { nullptr, 0, nullptr, 0 }
};

int main(int argc, char* argv[]) {
    getopt_long(argc, argv, "fb:", nullptr, nullptr);
    return 0;
}

