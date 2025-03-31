#include <iostream>

#include "MPointers.h"

int main() {
    MPointer<int>::Init("0.0.0.0:25565");

    MPointer<int> p1 = MPointer<int>::New();

    MPointer<int> p2 = MPointer<int>::New();

    std::cout << "p1: " << &p1 << std::endl;

    std::cout << "p2: " << &p2 << std::endl;

    *p1 = 5;

    int value = *p1;

    std::cout << "p1: " << value << std::endl;

    MPointer<int> p3;

    p3 = p1;
    
    return 0;
}
