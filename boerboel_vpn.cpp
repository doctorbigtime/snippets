#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <algorithm>

void usage(char const* pgm)
{
    std::cout << "Usage: " << pgm << " (start|stop|status|restart)"
        << std::endl;
    std::exit(1);
}

auto main(int argc, char**argv) -> int
{
    const char* commands[] = 
    {
        "start", "stop", "status", "restart"
    };
    if(argc != 2)
        usage(*argv);
    auto command = argv[1];
    if(std::find_if(commands, commands+4, [argv](char const* cmd) 
        {
        return 0 == std::strcmp(argv[1], cmd);
        }) == commands+4)
    {
        usage(*argv);
    }
    if(setuid(0))
    {
        std::cerr << "Could not setuid(0): " << strerror(errno) << std::endl;    
        return -1;
    }
    const char* path = "/home/sfortas/bin/oc";
    const char* args[] = { path, command };
    if(execv(path, argv))
    {
        std::cerr << "Could not execv(" << *argv << ", " << command 
                  << "): " 
                  << strerror(errno) << std::endl;    
        return -1;
    }
    return 0;
}
