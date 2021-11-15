#ifndef UNIX_DOMAIN_SOCKETS_H
#define UNIX_DOMAIN_SOCKETS_H

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <errno.h>
#include <string>

namespace ipc
{ 
namespace uds_datagram
{

class SocketNameRemover
{
public:

    SocketNameRemover(const std::string& socket_name)
    : m_socket_name(socket_name)
    {
        if(!remove())
        {
            std::ostringstream out;
            out << "cannot remove '" << m_socket_name << "'";
            throw std::runtime_error(out.str());
        }
    }

    ~SocketNameRemover()
    {
        if(!remove())
        {
            std::cout << "cannot remove '" << m_socket_name << "'" << std::endl;
        }
    }

    const std::string& name() const
    {
        return m_socket_name;
    }

private:

    bool remove()
    {
        //std::cout << "SocketNameRemover::remove(): removing '" << m_socket_name.c_str() << "'" << std::endl;

        int result = ::remove(m_socket_name.c_str());

        if((-1 == result) && (errno != ENOENT))
        {
            return false;
        }

        return true;
    }

    std::string m_socket_name;
};

const size_t DEFAULT_MSG_SIZE=1024;

const char* const UDS_DGRAM_SOCKET_NAME = "ipc_uds_dgram";

const char STOP_SERVER_FLAG = '#';

class Client
{
public:

    Client(const char* service_name, int64_t session_id)
    : m_name_remover(create_client_name(service_name, session_id))
    , m_sockfd(-1)
    {
        // client address
        ::memset(&m_client, 0, sizeof(struct sockaddr_un));
        m_client.sun_family = AF_UNIX;
        ::strncpy(m_client.sun_path, m_name_remover.name().c_str(), sizeof(m_client.sun_path)-1);

        // setup server address
        ::memset(&m_server, 0, sizeof(struct sockaddr_un));
        m_server.sun_family = AF_UNIX;
        ::strncpy(m_server.sun_path, service_name, sizeof(m_server.sun_path)-1);

        initiate();

        //std::cout << "initiate ipc client service=[" << m_server.sun_path << "] client=[" << m_client.sun_path << "]" << std::endl;
    }

    ~Client()
    {
        terminate();
    }

    static std::string create_client_name(const char* service_name, int64_t session_id)
    {
        // unique (service.source.session.pid)
        std::ostringstream out;
        out << service_name << "." << getpid() << "." << session_id;
        return out.str();
    }

    int read( char *buffer, const size_t size)
    {
        int read_size = ::recvfrom(m_sockfd, buffer, size, 0, 0, 0);

        if (-1 == read_size)
        {
            std::cout << "recvfrom socket syserr=[" << StrError().str() << "]" << std::endl;
        }

        return read_size;
    }

    bool write( const char *buffer, const size_t size)
    {
        socklen_t len = sizeof(struct sockaddr_un);

        size_t send_length = ::sendto(m_sockfd, buffer, size, 0, (struct sockaddr *) &m_server, len);

        if( size != send_length )
        {
            std::cout << "sendto socket syserr=[" << StrError().str() << "]" << std::endl;
            return false;
        }

        return true;
    }


    bool request( const char *request_buff
                , const size_t request_size
                , char *reply_buff
                , const size_t max_reply_size
                , int &reply_size
                )
    {
        if(!write(request_buff, request_size))
        {
            return false;
        }

        reply_size = read(reply_buff, max_reply_size);

        if(-1 == reply_size)
        {
            return false;
        }

        reply_buff[reply_size] = 0x0;

        /*
        // request
        int send_length = static_cast<int>( ::sendto( m_sockfd
                                                    , request_buff
                                                    , request_size
                                                    , 0
                                                    , (struct sockaddr *) &m_server
                                                    , static_cast<socklen_t>(sizeof(struct sockaddr_un))
                                                    ));

        if( send_length != (int)request_size)
        {
            std::ostringstream out;
            out << "sendto socket syserr=[" << StrError().str() << "]";
            std::cout << out.str() << std::endl;
            return false;
        }

        // reply
        reply_size = static_cast<int>( ::recvfrom( m_sockfd
                                                 , reply_buff
                                                 , max_reply_size
                                                 , 0
                                                 , 0
                                                 , 0
                                                 ));

        if (-1 == reply_size)
        {
            std::ostringstream out;
            out << "recvfrom socket syserr=[" << StrError().str() << "]";
            std::cout << out.str() << std::endl;
            return false;
        }

        reply[reply_size] = 0x0;
        */

        return true;
    }

    bool set_send_timeout(const size_t usec)
    {
        struct timeval tv;
        tv.tv_sec=(usec/1000000);
        tv.tv_usec=(usec%1000000);

        if( setsockopt(m_sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv, sizeof(tv)) )
        {
            std::cout << "setsockopt SO_SNDTIMEO syserr=[" << StrError().str() << "]" << std::endl;
            return false;
        }

        std::cout << "set setsockopt SO_SNDTIMEO sec=[" << tv.tv_sec << "] usec=[" << tv.tv_usec << "]" << std::endl;

        return true;
    }

    bool set_recv_timeout(const size_t usec)
    {
        struct timeval tv;
        tv.tv_sec=(usec/1000000);
        tv.tv_usec=(usec%1000000);

        if( setsockopt(m_sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv, sizeof(timeval)) )
        {
            std::cout << "setsockopt SO_RCVTIMEO syserr=[" << StrError().str() << "]" << std::endl;
            return false;
        }

        std::cout << "set setsockopt SO_RCVTIMEO sec=[" << tv.tv_sec << "] usec=[" << tv.tv_usec << "]" << std::endl;

        return true;
    }

private:

    void initiate()
    {
        // create socket
        m_sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

        if(-1 == m_sockfd)
        {
            std::ostringstream out;
            out << "create socket syserr=[" << StrError().str() << "]";
            throw std::runtime_error(out.str());
        }

        // bind
        if (bind(m_sockfd, (struct sockaddr *) &m_client, sizeof(struct sockaddr_un)) == -1)
        {
            std::ostringstream out;
            out << "bind socket syserr=[" << StrError().str() << "]";
            throw std::runtime_error(out.str());
        }
    }

    void terminate()
    {
        if( -1 != m_sockfd )
        {
            int ret = ::close(m_sockfd);
            if( -1 == ret )
            {
                std::cout << "socket close syserr=[" << StrError().str() << "]" << std::endl;
            }
            m_sockfd = -1;

            //std::cout << "terminate ipc client service=[" << m_server.sun_path << "] client=[" << m_client.sun_path << "]" << std::endl;
        }
    }

private:

    SocketNameRemover m_name_remover;

    int m_sockfd;
    struct sockaddr_un m_client;
    struct sockaddr_un m_server; 
};

///
/// server
///
class Server
{
public:

    Server(const char *service_name)
    : m_name_remover(service_name)
    , m_sockfd(-1)
    {
        // setup anonymous server address
        ::memset(&m_server, 0, sizeof(struct sockaddr_un));
        m_server.sun_family = AF_UNIX;
        ::strncpy(m_server.sun_path, service_name, sizeof(m_server.sun_path) - 1);

        //std::cout << "Server(): m_server.sun_path = " << m_server.sun_path << std::endl;

        initiate();
    }

    ~Server()
    {
        terminate();
    }

    int read( char *buffer, const size_t size, const struct sockaddr_un &client )
    {
        socklen_t len = sizeof(struct sockaddr_un);

        int actual = ::recvfrom(m_sockfd, buffer, size, 0, (struct sockaddr *)&client, &len);

        if (-1 == actual)
        {
            std::cout << "recvfrom socket syserr=[" << StrError() << "]" << std::endl;
        }

        return actual;
    }

    bool write( const char *buffer, const size_t size, const struct sockaddr_un &client )
    {
        socklen_t len = sizeof(struct sockaddr_un);

        size_t actual = sendto(m_sockfd, buffer, size, 0, (struct sockaddr *) &client,len);

        if( size != actual )
        {
            std::cout << "sendto socket syserr=[" << StrError().str() << "]" << std::endl;
            return false;
        }

        return true;
    }

    bool send_timeout(const size_t usec)
    {
        struct timeval tv;
        tv.tv_sec=(usec/1000000);
        tv.tv_usec=(usec%1000000);

        if( setsockopt(m_sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv, sizeof(tv)) )
        {
            std::cout << "setsockopt SO_SNDTIMEO syserr=[" << StrError().str() << "]" << std::endl;
            return false;
        }

        std::cout << "set setsockopt SO_SNDTIMEO sec=[" << tv.tv_sec << "] usec=[" << tv.tv_usec << "]" << std::endl;

        return true;
    }

    int listen_socket() const
    {
        return m_sockfd;
    }

private:

    ///
    /// start
    ///
    void initiate()
    {
        // create socket
        m_sockfd = ::socket(AF_UNIX, SOCK_DGRAM, 0);
        if (-1 == m_sockfd)
        {
            std::ostringstream out;
            out << "create socket syserr=[" << StrError() << "]";
            throw std::runtime_error(out.str());
        }

        // bind
        int ret = ::bind(m_sockfd, (struct sockaddr *) &m_server, sizeof(struct sockaddr_un));
        if( -1 == ret )
        {
            std::ostringstream out;
            out << "bind socket syserr=[" << StrError().str() << "]";
            throw std::runtime_error(out.str());
        }

        //std::cout << "initiate ipc server=[" << m_server.sun_path << "]" << std::endl;
    }

    void terminate()
    {
        if( -1 != m_sockfd )
        {
            int ret = ::close(m_sockfd);
            if( -1 == ret )
            {
                std::cout << "socket close syserr=[" << StrError() << "]" << std::endl;
            }
            m_sockfd = -1;
        }
    }

private:

    SocketNameRemover m_name_remover;
    int m_sockfd;
    struct sockaddr_un m_server;
};

} // namespace uds_datagram
} // namepsace ipc

#endif // UNIX_DOMAIN_SOCKETS_H
