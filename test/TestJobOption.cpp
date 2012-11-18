#include <gtest/gtest.h>

#include "ogl.h"
#include "Commands.h"

#include<ace/CDR_Stream.h>

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
using namespace std;

TEST(JobOption, constructor)
{
    ogl::JobOption* cmd = new ogl::JobOption();
    ASSERT_TRUE(cmd != 0);
}

TEST(JobOption, empty_serialize)
{
    ogl::JobOption* cmd = new ogl::JobOption();
    ASSERT_TRUE(cmd != 0);
    ACE_Message_Block* msg = cmd->serialize();
    ASSERT_EQ(msg->length() , 7 * sizeof (ACE_CDR::ULong));

    delete cmd;
    msg->release();
}

TEST(JobOption, serialize)
{
    ogl::JobOption* jobOption = new ogl::JobOption();

    jobOption->name("dma");
    jobOption->command("/bin/date");

    ACE_Message_Block* msg = jobOption->serialize();

    ogl::JobOption* target = new ogl::JobOption();
    target->deserialize(msg);

    ASSERT_EQ(std::string("dma"), target->name());

    delete jobOption;
    delete target;
    msg->release();
}


TEST(JobOption, copyConstructor)
{
    ogl::JobOption job;
    job.name("dma");
    job.command("/bind/date");

    ogl::JobOption job1(job);

    ASSERT_EQ(std::string(job.name()), job1.name());
    ASSERT_EQ(std::string(job.command()), job1.command());

    ogl::JobOption job2;
    job2.name("dma");
    job2.command("/bin/ls");
    job2.name("dma1982ZZ");

    job1 = job2;


    ASSERT_EQ(std::string(job1.name()), job2.name());
    ASSERT_EQ(std::string(job1.command()), job2.command());

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
