#include <iostream>
#include "arguments.h"

int main(int argc, char *argv[]) {
    Arguments arguments = parse_arguments(argc, argv);
    std::cout << "Hello, World!\n";
    return 0;
}
