#include "uds_dgram.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

const char* words1[] = {"alpha", "_beta", "gamma", "delta"};
const char* words2[] = {"__one", "__two", "three", "_four"};

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "not enough params" << std::endl;
        return EXIT_FAILURE;
    }

    int ver = std::atoi(argv[1]);

    size_t word_len   = strlen(words1[0]);
    size_t words_count = sizeof(words1)/sizeof(words1[0]);

    const char** words = (1 == ver ? words1 : words2);

    try
    {
        using namespace ipc::uds_datagram;

        int session_id = 123;
        std::cout << "starting client with servce=[" << UDS_DGRAM_SOCKET_NAME << "] session_id=[" << session_id << "]" << std::endl;

        Client client(UDS_DGRAM_SOCKET_NAME, session_id);

        const size_t total_num_of_requests = 100;

        for(size_t i = 0; i < total_num_of_requests; ++i)
        {
            for(size_t iWord = 0; iWord < words_count; ++iWord)
            {
                std::cout << "request=[" << words[iWord] << "]" << std::endl;

                int reply_size=0;
                char reply[DEFAULT_MSG_SIZE] = {0};

                client.request(words[iWord],(int)word_len,reply,sizeof(reply)-1,reply_size);

                std::cout << "reply=[" << reply << "]" << std::endl;

                usleep(200 * 1000); // sleep for 200 us
            }
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cout << argv[0] << ". ERROR: the std::runtime_error has been thrown. descr = '" << e.what() << "'." << std::endl;
    }

    return EXIT_SUCCESS;
}
