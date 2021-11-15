#include "uds_dgram.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int /*argc*/, char** argv)
{
    using namespace ipc::uds_datagram;

    try
    {
        Server server(UDS_DGRAM_SOCKET_NAME);

        while( true )
        {
            char buffer[DEFAULT_MSG_SIZE] = {0};
            struct sockaddr_un client;
            int size = server.read(buffer,sizeof(buffer)-1,client);
            if( -1 == size )
            {
                std::cout << "error reading socket" << std::endl;
                continue;
            }

            std::cout << "server::read(): size = " << size << " buffer << [" << buffer << "]" << std::endl;

            if( false == server.write(buffer,size,client) )
            {
                std::cout << "error writing socket" << std::endl;
                continue;
            }
        }

        std::cout << "server has terminated" << std::endl;;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << argv[0] << ". ERROR: the std::runtime_error has been thrown. descr = '" << e.what() << "'." << std::endl;
    }

    return EXIT_SUCCESS;
}
