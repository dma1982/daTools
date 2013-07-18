#include "ogl.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

TEST(ConsistentHashNet, addNode)
{
    ogl::ConsistentHashNet hashNet;
    hashNet.addNode("hello world");
    cout << hashNet.getNode("hello") << endl;
}
