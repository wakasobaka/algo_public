#include "uds_dgram.h"

#include <cstdlib>
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

    std::cout << "message size: " << size << " bytes" << std::endl;
    std::cout << "message count: " << count << std::endl;

    using namespace ipc::uds_datagram;

    try
    {
        if (!fork())
        {
            /*child - server*/

            //std::cout << "starting client with servce=[" << UDS_DGRAM_SOCKET_NAME << "] session_id=[" << session_id << "]" << std::endl;

            Server server(UDS_DGRAM_SOCKET_NAME);

            char buffer[DEFAULT_MSG_SIZE] = {0};

            for (int i = 0; i < count; i++)
            {
                struct sockaddr_un client;
                int read_size = server.read(buffer,sizeof(buffer)-1,client);
                if( -1 == read_size )
                {
                    std::cout << "error reading socket" << std::endl;
                    continue;
                }

                //std::cout << "server::read(): size = " << size << " buffer << [" << buffer << "]" << std::endl;

                if( false == server.write(buffer,read_size,client) )
                {
                    std::cout << "error writing socket" << std::endl;
                    continue;
                }

            }
        }
        else
        {
            /*parent - client*/

            sleep(1);

            int session_id = 123;
            //std::cout << "starting client with servce=[" << UDS_DGRAM_SOCKET_NAME << "] session_id=[" << session_id << "]" << std::endl;

            Client client(UDS_DGRAM_SOCKET_NAME, session_id);

            char request_buff[DEFAULT_MSG_SIZE] = {0};
            char reply_buff[DEFAULT_MSG_SIZE] = {0};

            int reply_size=0;

            uint64_t ustart = ipc::time_utils::get_hr_time_ns();

            for (int i = 0; i < count; i++)
            {
                client.request( request_buff
                              , DEFAULT_MSG_SIZE - 1
                              , reply_buff
                              , DEFAULT_MSG_SIZE - 1
                              , reply_size
                              );

            }

            uint64_t ustop = ipc::time_utils::get_hr_time_ns();

            int64_t udelta = ipc::time_utils::time_elapsed_ns(ustop, ustart);

            std::cout << "average latency: " <<  udelta / (count * 2 * 1000) << " us" << std::endl;;
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cout << argv[0] << ". ERROR: the std::runtime_error has been thrown. descr = '" << e.what() << "'." << std::endl;
    }


    return EXIT_SUCCESS;
}
