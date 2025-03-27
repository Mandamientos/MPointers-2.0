#include <iostream>

#include "MPointers.h"

int main() {
    MPointer<int>::Init("0.0.0.0:25565");

    MPointer<int> p = MPointer<int>::New();

    std::cout << p.id_ << std::endl;
    
    return 0;
}
