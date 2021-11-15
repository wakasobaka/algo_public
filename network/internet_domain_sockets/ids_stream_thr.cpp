#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <stdint.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/wait.h>

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

    int ret = 0;
    if( (ret = ::getaddrinfo("127.0.0.1", "3491", &hints, &res)) != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(ret) << std::endl;
        return EXIT_FAILURE;
    }

    int sockfd = 0, new_fd = 0;
    socklen_t addr_size = 0;
    struct sockaddr_storage their_addr = {0};
    int yes = 1;

    pid_t child_pid = ::fork();

    switch(child_pid)
    {
    case -1: // error

        std::cerr << argv[0] << ": cannot create child process" << std::endl;
        return EXIT_FAILURE;

    break;

    case  0: // child
    {
        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            std::cerr << "child/socket: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            std::cerr << "child/setsockopt: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if (::bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            std::cerr << "child/bind: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        // The listen function enables the socket socket to accept connections,
        // thus making it a server socket.
        if (::listen(sockfd, 1) == -1)
        {
            std::cerr << "child/listen: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        addr_size = sizeof(their_addr);

        // This function is used to accept a connection request on the server socket socket.
        if ((new_fd = ::accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
        {
            std::cerr << "child/accept: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        for (size_t sofar = 0; sofar < (count * size);)
        {
            ssize_t len = ::read(new_fd, &buf[0], size);
            if (len == -1)
            {
                std::cerr << "ERROR: read" << StrError().str() << std::endl;
                return EXIT_FAILURE;
            }
            sofar += len;
        }

        ::shutdown(sockfd, SHUT_RD);
        ::close(sockfd);
    }
    break;

    default:
    {
        // parent

        ::sleep(1);

        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            std::cerr << "parent/socket: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if(::connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            std::cerr << "parent/connect: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        if(::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) == -1)
        {
            std::cerr << "parent/setsockopt: " << StrError().str() << std::endl;
            return EXIT_FAILURE;
        }

        uint64_t ustart = ipc::time_utils::get_hr_time_ns();

        for (int i = 0; i < count; i++)
        {
            if(::write(sockfd, &buf[0], size) != size)
            {
                std::cerr << "ERROR: write" << StrError().str() << std::endl;
                return EXIT_FAILURE;
            }
        }

        uint64_t ustop = ipc::time_utils::get_hr_time_ns();

        ::shutdown(sockfd, SHUT_WR);
        ::close(sockfd);
        
        int64_t udelta = ipc::time_utils::time_elapsed_ns(ustop, ustart); 
        
        // messages / second
        // count - udelate (ns)
        // x     - NANOSECS_IN_SEC
        std::cout << "average throughput: " << (count * ipc::time_utils::NANOSECS_IN_SEC) / udelta << " msg/s" << std::endl;
        
        // MB / seconds
        // size * count (bytes) / MEGABYTE - udelta (ns)
        // x                               - NANOSECS_IN_SEC
        std::cout << "average throughput: " << (size * count * ipc::time_utils::NANOSECS_IN_SEC) / (ipc::MEGABYTE * udelta) << " Mb/s" << std::endl;

        std::cout << "Waiting child process to complete" << std::endl;

        int status = EXIT_SUCCESS;

        ::waitpid(child_pid, &status, 0);
    }
    break;
    }

    return EXIT_SUCCESS;
}
