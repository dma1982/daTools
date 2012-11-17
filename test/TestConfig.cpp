#include <gtest/gtest.h>
#include "ogl.h"

#include <string>

#include <iostream>

using namespace std;

TEST(Configuration, constructor)
{
    ogl::Configuration* conf = ogl::Configuration::instance();
    conf->read("./ogl.conf");
    ASSERT_EQ(conf->getMasterCliPort() , 9880);
    ASSERT_EQ(conf->getMasterJrPort() , 9881);
    ASSERT_EQ(string("9.123.145.253"), conf->getMasterHost());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
