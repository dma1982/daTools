#include <gtest/gtest.h>
#include "ogl.h"

#include "Commands.h"

#include <string>

#include <iostream>

using namespace std;


TEST(CommandHeader, constructor)
{
    ogl::CommandHeader header(ogl::CreateJobCommand, "aaa");
}

TEST(CommandHeader, serialize)
{
    ogl::CommandHeader header(ogl::CreateJobCommand, "bbb");
    ACE_Message_Block* msg = header.serialize();
    msg->release();
}

TEST(CommandHeader, contextId)
{
    char* buf = new char[1024];
    sprintf(buf, "%s", "bbbb");
    ogl::CommandHeader header(ogl::CreateJobCommand, buf);
    delete[] buf;
    ACE_Message_Block* msg = header.serialize();
    msg->release();
}

TEST(CommandHeader, deserialize)
{
    ogl::CommandHeader header(ogl::CreateJobCommand, "bbb");
    ACE_Message_Block* msg = header.serialize();

    ogl::CommandHeader header_2(ogl::CreateJobCommand);
    header_2.deserialize(msg);
    msg->release();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

