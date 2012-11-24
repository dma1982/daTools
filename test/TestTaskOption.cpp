#include <gtest/gtest.h>

#include "ogl.h"

#include "Commands.h"

#include<ace/CDR_Stream.h>

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
using namespace std;

TEST(TaskOption, constructor)
{
    ogl::TaskOption* cmd = new ogl::TaskOption();
    ASSERT_TRUE(cmd != 0);
}

TEST(TaskOption, empty_serialize)
{
    ogl::TaskOption* cmd = new ogl::TaskOption();
    ASSERT_TRUE(cmd != 0);
    ACE_Message_Block* msg = cmd->serialize();
    ASSERT_EQ(msg->length() , 20);

    delete cmd;
    msg->release();
}

TEST(TaskOption, serialize)
{
    ogl::TaskOption* taskOption = new ogl::TaskOption();

    ogl::Buffer input("iii", 4);
    ogl::Buffer output("ooo", 4);

    taskOption->jobName("test");
    taskOption->taskInput(input);
    taskOption->taskOutput(output);

    ACE_Message_Block* msg = taskOption->serialize();

    ogl::TaskOption* target = new ogl::TaskOption();
    target->deserialize(msg);

    ASSERT_EQ(std::string("test"), target->jobName());
    ASSERT_EQ(std::string("ooo"), target->taskOutput().data());

    delete taskOption;
    delete target;
    msg->release();

}


TEST(TaskOption, copyConstructor)
{
    ogl::TaskOption task;
    ogl::Buffer output("ooo", 4);
    task.jobName("dma");
    task.taskOutput(output);

    ogl::TaskOption task1(task);

    ASSERT_EQ(std::string(task.jobName()), task1.jobName());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
