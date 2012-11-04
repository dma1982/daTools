#include <gtest/gtest.h>

#include "ogl.h"
#include "Commands.h"

#include<ace/CDR_Stream.h>

#include <iostream>
#include <stdio.h>

TEST(CreateJobCommand, constructor)
{
    ogl::Command* cmd = new ogl::CreateJob();
    ASSERT_TRUE(cmd != 0);
}

TEST(CreateJobCommand, empty_serialize)
{
    ogl::Command* cmd = new ogl::CreateJob();
    ASSERT_TRUE(cmd != 0);
    ACE_Message_Block* msg = cmd->serialize();
    ASSERT_TRUE(msg->length() == sizeof(ogl::Command));

    ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

    os << ACE_CDR::ULong(ogl::CreateJobCommand);

    ASSERT_TRUE(memcmp(msg->rd_ptr(), os.begin()->rd_ptr(), sizeof(ogl::Command)) == 0);

    msg->release();
    delete cmd;
}


TEST(CreateJobCommand, serialize)
{
    ogl::Command* cmd = new ogl::CreateJob();
    ASSERT_TRUE(cmd != 0);
    ACE_Message_Block* msg = cmd->serialize();

    ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

    os << ACE_CDR::ULong(ogl::CreateJobCommand);

    ASSERT_TRUE(memcmp(msg->rd_ptr(), os.begin()->rd_ptr(), sizeof(ogl::Command)) == 0);

    msg->release();
    delete cmd;

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
