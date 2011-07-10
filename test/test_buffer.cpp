#include "sw/buffer.h"

#include <iostream>

using namespace sw;
using namespace std;

int main(int argc, char** argv)
{
    buffer_t buf(2);

    buf.append("123", 4);

    *(buf.data()+4) = 0;

    cout << (buf.data()) << endl;
    cout << buf.size() << endl;
    cout << buf.capacity() << endl;

    cout << buf.next() -> data() << endl;
    cout << buf.next() -> size() << endl;
    cout << buf.next() -> capacity() << endl;

    return 0;
}
