#include <iostream>

#include <sflog/sflog.h>

int some_complex_operation() {
    int i = 0;
    std::cin >> i;
    return i;
}

int main() {
    LOG_ERROR("Some ERROR {}", some_complex_operation());
    LOG_WARNING("Some WARNING {}", some_complex_operation());
    LOG_INFO("Some INFO {}", some_complex_operation());
    LOG_DEBUG("Some DEBUG {}", some_complex_operation());
    return 0;
}
