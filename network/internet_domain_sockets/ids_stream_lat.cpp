#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <stdint.h>
#include <netdb.h>

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

    struct addrinfo *res = 0;
    struct addrinfo hints = {0};

    hints.ai_family   = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;     // fill in my IP for me

    if( ::getaddrinfo("127.0.0.1", "3491", &hints, &res) != 0)
    {
        std::cerr << "getaddrinfo: " << StrError().str() << std::endl;
        return EXIT_FAILURE;
    }

    int sockfd = 0, new_fd = 0;
    socklen_t addr_size = 0;
    struct sockaddr_storage their_addr = {0};
    int yes = 1;

    if (!fork())
    {
        // child

        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            std::cerr << "socket: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            std::cerr << "setsockopt: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if (::bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            std::cerr << "bind: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if (::listen(sockfd, 1) == -1)
        {
            std::cerr << "listen: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        addr_size = sizeof(their_addr);

        if ((new_fd = ::accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
        {
            std::cerr << "accept: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        for (int i = 0; i < count; i++)
        {
            for (int sofar = 0; sofar < size; )
            {
                ssize_t len = ::read(new_fd, &buf[0], size - sofar);
                if (len == -1)
                {
                    std::cerr << "ERROR: read" << StrError().str() << std::endl;
                    return EXIT_FAILURE;
                }
                sofar += len;
            }

            if(::write(new_fd, &buf[0], size) != size)
            {
                std::cerr << "ERROR: write" << StrError().str() << std::endl;
                return EXIT_FAILURE;
            }
        }
    } 
    else 
    {
        /* parent */

        ::sleep(1);

        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            std::cerr << "socket: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if(::connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            std::cerr << "connect: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        uint64_t ustart = ipc::time_utils::get_hr_time_ns();

        for (int i = 0; i < count; i++)
        {
            if (::write(sockfd, &buf[0], size) != size)
            {
                std::cerr << "ERROR: write" << StrError().str() << std::endl;
                return EXIT_FAILURE;
            }

            for (int sofar = 0; sofar < size; )
            {
                ssize_t len = ::read(sockfd, &buf[0], size - sofar);
                if (len == -1)
                {
                    std::cerr << "ERROR: read" << StrError().str() << std::endl;
                    return EXIT_FAILURE;
                }
                sofar += len;
            }
        }

        uint64_t ustop = ipc::time_utils::get_hr_time_ns();
        
        int64_t udelta = ipc::time_utils::time_elapsed_ns(ustop, ustart); 
        
        std::cout << "average latency: " <<  udelta / (count * 2 * 1000) << " us" << std::endl;;
    }

    return EXIT_SUCCESS;
}
