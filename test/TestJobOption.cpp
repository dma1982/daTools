#include <gtest/gtest.h>

#include "ogl.h"
#include "Commands.h"

#include<ace/CDR_Stream.h>

#include <iostream>
#include <stdio.h>
#include <cstring>
using namespace std;

TEST(JobOption, constructor)
{
    ogl::JobOption* cmd = new ogl::JobOption();
    ASSERT_TRUE(cmd !=0);
}

TEST(JobOption, empty_serialize)
{
    ogl::JobOption* cmd = new ogl::JobOption();
    ASSERT_TRUE(cmd !=0);
    ACE_Message_Block* msg = cmd->serialize();
    ASSERT_EQ(msg->length() , 7 * sizeof (ACE_CDR::ULong));

    delete cmd;
    msg->release();
}

int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
