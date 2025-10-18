
#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    using socklen_t = int;

    inline void init_system() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw std::runtime_error("WSAStartup failed");
    }

    inline void cleanup_system() { WSACleanup(); }
    inline void close_socket(SOCKET _socket) { closesocket(_socket); }
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>

    using SOCKET = int;
    inline const int INVALID_SOCKET = -1;
    inline const int SOCKET_ERROR   = -1;

    inline void init_system() {}
    inline void cleanup_system() {}
    inline void close_socket(SOCKET s) 
    { close(s); }
#endif

class jackwarp 
{
protected:
    SOCKET _socket;
    SOCKET _jack = INVALID_SOCKET;

    jackwarp();
    ~jackwarp();

public:
    jackwarp& operator<<(const std::string&);
    jackwarp& operator>>(std::string&);
};


class jackhost: public jackwarp 
{
    sockaddr_in host_address{};
public:
    explicit jackhost(); // добавил конструктор по умолчанию
    void jackhost_act(unsigned short); // отредачил функцию инициализации
    void wait_jack();
};


class jack: public jackwarp 
{
    sockaddr_in host_address{};
public:
    jack(); // добавил конструктор по умолчанию
    void jack_act(const std::string&, unsigned short);
};
