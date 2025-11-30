
#include "jack.h"

jackwarp::jackwarp() {
    init_system();
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == INVALID_SOCKET)
        throw JackNetworkError("socket creation failed");
}

jackwarp::~jackwarp() {
    if (_jack   != INVALID_SOCKET) close_socket(_jack);
    if (_socket != INVALID_SOCKET) close_socket(_socket);
    cleanup_system();
}

jackwarp& jackwarp::operator<<(const std::string& data) {
    if (_jack == INVALID_SOCKET)
        throw JackNetworkError("no jack connected");

    int sent = send(_jack, data.c_str(), (int)data.size(), 0);
    if (sent == SOCKET_ERROR)
        throw JackNetworkError("send failed");

    return *this;
}

jackwarp& jackwarp::operator>>(std::string& out) {
    if (_jack == INVALID_SOCKET)
        throw JackNetworkError("no jack connected");

    char buffer[1024] = {0};
    int bytes = recv(_jack, buffer, sizeof(buffer) - 1, 0);
    if (bytes == SOCKET_ERROR)
        throw JackNetworkError("recv failed");

    if (bytes > 0) {
        buffer[bytes] = '\0';
        out.assign(buffer);
    } else {
        out.clear();
    }
    return *this;
}


// ------------------------ jackhost ------------------------

jackhost::jackhost(): jackwarp()
{}

void jackhost::jackhost_act(unsigned short port) {
    host_address.sin_family = AF_INET;
    host_address.sin_addr.s_addr = htonl(INADDR_ANY);
    host_address.sin_port = htons(port);

    if (bind(_socket, reinterpret_cast<sockaddr*>(&host_address),
             sizeof(host_address)) == SOCKET_ERROR)
        throw JackNetworkError("bind failed");

    if (listen(_socket, 1) == SOCKET_ERROR)
        throw JackNetworkError("listen failed");
}

void jackhost::wait_jack() {
    sockaddr_in jack_address{};
    socklen_t jack_address_size = sizeof(jack_address);

    _jack = accept(_socket, reinterpret_cast<sockaddr*>(&jack_address),
                   &jack_address_size);
    if (_jack == INVALID_SOCKET)
        throw JackNetworkError("accept failed");
}

std::string jackhost::get_ip() const {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
        throw JackNetworkError("gethostname() failed");

    addrinfo hints{}, *info = nullptr;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (getaddrinfo(hostname, nullptr, &hints, &info) != 0)
        throw JackNetworkError("getaddrinfo() failed");

    char ip[INET_ADDRSTRLEN];
    auto* addr = reinterpret_cast<sockaddr_in*>(info->ai_addr);
    if (!inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip))) {
        freeaddrinfo(info);
        throw JackAddressError("inet_ntop() failed converting IP");
    }

    freeaddrinfo(info);
    return ip;
}

std::string jackhost::get_invite() const {
    std::string ip = get_ip();
    in_addr addr{};
    if (inet_pton(AF_INET, ip.c_str(), &addr) != 1)
        throw JackAddressError("inet_pton() failed while encoding invite");

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&addr);
    std::ostringstream code;
    for (int i = 0; i < 4; ++i)
        code << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i];

    return code.str();
}


// ------------------------ jack ------------------------

jack::jack(): jackwarp()
{}

void jack::jack_act(const std::string& host, unsigned short port) {
    host_address.sin_family = AF_INET;
    host_address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &host_address.sin_addr) <= 0)
        throw JackAddressError("invalid host address");

    if (connect(_socket, reinterpret_cast<sockaddr*>(&host_address),
                sizeof(host_address)) == SOCKET_ERROR)
        throw JackNetworkError("connect failed");

    _jack = _socket;
}

std::string jack::invite_to_ip(const std::string& code) {
    if (code.size() != 8)
        throw JackInviteError("invalid invite code length (expected 8 hex chars)");

    uint8_t bytes[4];
    for (int i = 0; i < 4; ++i) {
        try {
            bytes[i] = static_cast<uint8_t>(
                std::stoi(code.substr(i * 2, 2), nullptr, 16)
            );
        } catch (const std::invalid_argument&) {
            throw JackInviteError("invite contains non-hex characters");
        } catch (const std::out_of_range&) {
            throw JackInviteError("invite contains out-of-range byte");
        }
    }

    in_addr addr{};
    std::memcpy(&addr, bytes, 4);

    char buffer[INET_ADDRSTRLEN];
    if (!inet_ntop(AF_INET, &addr, buffer, sizeof(buffer)))
        throw JackAddressError("inet_ntop() failed while decoding invite");

    return buffer;
}
