#include <gtest/gtest.h>

#include "ogl.h"
#include "jm/Commands.h"

TEST(CreateJobCommand, constructor)
{
    ogl::Command* cmd = new ogl::CreateJob();
}


int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
