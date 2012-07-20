#include "sw.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    sw::Buffer buf(2);

    buf.append("123", 4);

    *(buf.data()+4) = 0;

    std::cout << (buf.data()) << std::endl;
    cout << buf.size() << endl;
    cout << buf.capacity() << endl;

    cout << buf.next() -> data() << endl;
    cout << buf.next() -> size() << endl;
    cout << buf.next() -> capacity() << endl;

    return 0;
}
