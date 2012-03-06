#include "log.h"

using namespace sw;

int main(int argc, char** argv)
{
    Logger* log = Logger::getLogger("mada.class");
    log->Debug("this is the first line");
    log->Assert(1!=1);
}
