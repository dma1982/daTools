#include <gtest/gtest.h>
#include "ogl.h"

#include <string>

#include <iostream>

using namespace std;

TEST(Configuration, constructor)
{
    ogl::Configuration* conf = ogl::Configuration::instance();
    conf->read("./ogl.conf");
    ASSERT_EQ(conf->getMasterPort() , 9080);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
