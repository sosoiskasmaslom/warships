#include <iostream>
#include <string>


#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    using socklen_t = int;

    void init_system() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw std::runtime_error("WSAStartup failed");
    }

    void cleanup_system() { WSACleanup(); }
    void close_socket(SOCKET _socket) { closesocket(_socket); }
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>

    using SOCKET = int;
    const int INVALID_SOCKET = -1;
    const int SOCKET_ERROR   = -1;

    void init_system() {}
    void cleanup_system() {}
    void close_socket(SOCKET s) {
        close(s);
    }
#endif


class jackwarp {
protected:
    SOCKET _socket;
    SOCKET _jack = INVALID_SOCKET;

    jackwarp() {
        init_system();
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == INVALID_SOCKET)
            throw std::runtime_error("jackwarp failed");
    }

    ~jackwarp() {
        if (_jack != INVALID_SOCKET)   close_socket(_jack);
        if (_socket != INVALID_SOCKET) close_socket(_socket);
        cleanup_system();
    }

public:
    jackwarp& operator<<(const std::string& data) {
        if (_jack == INVALID_SOCKET)
            throw std::runtime_error("no jack connected");

        int sent = send(_jack, data.c_str(), (int)data.size(), 0);
        if (sent == SOCKET_ERROR)
            throw std::runtime_error("jackhost send failed");

        return *this;
    }

    jackwarp& operator>>(std::string& out) {
        if (_jack == INVALID_SOCKET)
            throw std::runtime_error("no jack connected");

        char buffer[1024] = {0};
        int bytes = recv(_jack, buffer, sizeof(buffer) - 1, 0);
        if (bytes == SOCKET_ERROR)
            throw std::runtime_error("jackhost recv failed");

        if (bytes > 0) {
            buffer[bytes] = '\0';
            out.assign(buffer);
        } else {
            out.clear();
        }

        return *this;
    }
};


class jackhost: public jackwarp {
    sockaddr_in host_address{};
public:
    explicit jackhost(unsigned short port): jackwarp() {
        host_address.sin_family = AF_INET;
        host_address.sin_addr.s_addr = htonl(INADDR_ANY);
        host_address.sin_port = htons(port);

        if (bind(
            _socket,
            reinterpret_cast<sockaddr*>(&host_address),
            sizeof(host_address)
        ) == SOCKET_ERROR)
            throw std::runtime_error("jackhost bind failed");

        if (listen(_socket, 1) == SOCKET_ERROR)
            throw std::runtime_error("jackhost listen failed");
    }

    void wait_jack() {
        sockaddr_in jack_address;
        socklen_t jack_address_size = sizeof(jack_address);

        _jack = accept(
            _socket,
            reinterpret_cast<sockaddr*>(&jack_address),
            &jack_address_size
        );

        if (_jack == INVALID_SOCKET)
            throw std::runtime_error("jackhost wait_jack failed");
    }
};


class jack: public jackwarp {
    sockaddr_in host_address{};

public:
    jack(const std::string& host, unsigned short port): jackwarp() {
        host_address.sin_family = AF_INET;
        host_address.sin_port = htons(port);

        if (inet_pton(AF_INET, host.c_str(), &host_address.sin_addr) <= 0)
            throw std::runtime_error("jackhost address wrong");

        if (connect(
            _socket,
            reinterpret_cast<sockaddr*>(&host_address),
            sizeof(host_address)
        ) == SOCKET_ERROR)
            throw std::runtime_error("jack connect failed");

        _jack = _socket;
    }
};
