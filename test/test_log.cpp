#include "log.h"

using namespace sw;

int main(int argc, char** argv)
{
    Logger* log = Logger::getLogger("mada.class");
    log->Debug("this is the first line");
    //log->Debug("this is the %s line", "second");
    log->Assert(1!=1);
    log->printStackTrace();
}
