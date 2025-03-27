#include <iostream>

#include "MPointers.h"

int main() {
    MPointer<int>::Init("0.0.0.0:25565");

    MPointer<int> p = MPointer<int>::New();
    
    return 0;
}
