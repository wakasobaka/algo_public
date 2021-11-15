#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <stdint.h>

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " <message-size> <message-count>" << std::endl;
        return EXIT_FAILURE;
    }

    int size = std::atoi(argv[1]);
    int64_t count = std::atol(argv[2]);

    std::vector<char> buf(size);

    std::cout << "message size: " << size << " bytes" << std::endl;
    std::cout << "message count: " << count << std::endl;

    int fds[2] = {0}; /* the pair of socket descriptors */
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1)
    {
        std::cerr << "ERROR: socketpair" << std::endl;
        return EXIT_FAILURE;
    }

    if (!fork())
    {
        /* child */

        for (int i = 0; i < count; i++)
        {
            if (read(fds[1], &buf[0], size) != size)
            {
                std::cerr << "ERROR: read" << std::endl;
                return EXIT_FAILURE;
            }
        }
    } 
    else 
    {
        /* parent */

        uint64_t ustart = ipc::time_utils::get_hr_time_ns();

        for (int i = 0; i < count; i++)
        {
            if (write(fds[0], &buf[0], size) != size)
            {
                std::cerr << "ERROR: write" << std::endl;
                return EXIT_FAILURE;
            }
        }

        uint64_t ustop = ipc::time_utils::get_hr_time_ns();
        
        int64_t udelta = ipc::time_utils::time_elapsed_ns(ustop, ustart); 
        
        // messages / second
        // count - udelate (ns)
        // x     - NANOSECS_IN_SEC
        std::cout << "average throughput: " << (count * ipc::time_utils::NANOSECS_IN_SEC) / udelta << " msg/s" << std::endl;
        
        // MB / seconds
        // size * count (bytes) / MEGABYTE - udelta (ns)
        // x                               - NANOSECS_IN_SEC
        std::cout << "average throughput: " << (size * count * ipc::time_utils::NANOSECS_IN_SEC) / (ipc::MEGABYTE * udelta) << " Mb/s" << std::endl;
    }

    return EXIT_SUCCESS;
}
