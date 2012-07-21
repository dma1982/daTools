#include "JobManager.h"



int main(int argc, char** argv)
{
    JobManager::instance() -> start();

    return 0;
}


