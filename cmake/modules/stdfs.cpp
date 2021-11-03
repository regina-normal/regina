
#if __has_include(<filesystem>)
  #include <filesystem>
  using filesystem_path = std::filesystem::path;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  using filesystem_path = std::experimental::filesystem::path;
#endif

#include <iostream>

int main() {
    filesystem_path p("/usr/bin/foo");
    std::cout << p.filename() << std::endl;
    return 0;
}

