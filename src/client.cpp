#include <iostream>
#include "LinkedList.h"

int testCase () {

    MPointer<int> p1 = MPointer<int>::New();
    MPointer<int> p2 = MPointer<int>::New();

    *p1 = 10;
    *p2 = 20;

    std::cout << "p1: " << *p1 << std::endl;
    std::cout << "p2: " << *p2 << std::endl;

    p1 = p2;

    std::cout << "p1: " << *p1 << std::endl;
    std::cout << "p2: " << *p2 << std::endl;

    return 0;
}

int linkedListTestCase() {

    MPointer<Node<int>>::Init("0.0.0.0:25565");
    std::cout << "Init done\n";

    LinkedList<int> list;

    list.push_back(10);

    list.push_back(20);

    list.push_back(30);

    list.print();

    return 0;

}

int main() {
    MPointer<int>::Init("0.0.0.0:25565");

    testCase();

    //linkedListTestCase();

    return 0;
}

