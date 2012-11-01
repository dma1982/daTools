#include <gtest/gtest.h>

#include "ogl.h"
#include "jm/Commands.h"

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


TEST(JobOption, serialize)
{
    ogl::CreateJob* cmd = new ogl::CreateJob();
    ASSERT_TRUE(cmd !=0);
    ogl::JobOption* jobOption = new ogl::JobOption();
    cmd->setJobOption(jobOption);
    jobOption->priority(1);
    char* name = new char[128];
    strncpy(name, "dma", 4);
    jobOption->name(name);

    ACE_Message_Block* msg = cmd->serialize();

    ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

    os << ACE_CDR::ULong(ogl::CreateJobCommand);

    ASSERT_TRUE(memcmp(msg->rd_ptr(), os.begin()->rd_ptr(), sizeof(ogl::Command)) == 0);

    ogl::CreateJob* c = new ogl::CreateJob();
    c->deserialize(msg);

    ASSERT_EQ(strcmp(name, c->getJobOption()->name()), 0);
    ASSERT_EQ(c->getJobOption()->priority(), 1);

    msg->release();
    delete cmd;
    delete c;
}

TEST(JobOption, deserialize)
{
    ogl::CreateJob* cmd = new ogl::CreateJob();
    ASSERT_TRUE(cmd !=0);
    ogl::JobOption* jobOption = new ogl::JobOption();
    cmd->setJobOption(jobOption);
    jobOption->priority(1);
    char* name = new char[128];
    strncpy(name, "dma", 4);
    jobOption->name(name);

    ACE_Message_Block* msg = cmd->serialize();

    ACE_OutputCDR os(ACE_DEFAULT_CDR_BUFSIZE);

    os << ACE_CDR::ULong(ogl::CreateJobCommand);

    ASSERT_TRUE(memcmp(msg->rd_ptr(), os.begin()->rd_ptr(), sizeof(ogl::Command)) == 0);

    msg->release();
    delete cmd;
}


int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
