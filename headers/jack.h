
#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include "jackexcept.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    using socklen_t = int;
    /// \brief Инициализация WinSock (выполняется один раз).
    inline static void init_system() noexcept {
        static bool initialized = false;
        if (!initialized) {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
                throw JackNetworkError("WSAStartup failed");
            initialized = true;
            std::atexit([] { WSACleanup(); });
        }
    }
    inline void cleanup_system() noexcept { WSACleanup(); }
    inline static void close_socket(SOCKET s) noexcept { closesocket(s); }
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    using SOCKET = int;
    inline const int INVALID_SOCKET = -1;
    inline const int SOCKET_ERROR   = -1;
    inline static void init_system() noexcept {}
    inline static void cleanup_system() noexcept {}
    inline static void close_socket(SOCKET s) noexcept { close(s); }
#endif


//======================================================================
// class jackwarp
//----------------------------------------------------------------------
/// \brief Базовый класс для TCP-соединений.
///
/// Оборачивает низкоуровневый сокет и предоставляет базовые операции
/// ввода-вывода. Не используется напрямую.
/// Используется через классы-наследники jackhost и jack.
///
class jackwarp {
protected:
    SOCKET _socket = INVALID_SOCKET; ///< Основной сокет.
    SOCKET _jack   = INVALID_SOCKET; ///< Активное соединение (accept/connect).

public:
    /// \brief Создаёт TCP-сокет (AF_INET, SOCK_STREAM).
    ///
    /// На Windows автоматически инициализирует WinSock.
    /// \throws JackNetworkError при ошибке создания сокета.
    jackwarp();

    /// \brief Закрывает все открытые сокеты.
    virtual ~jackwarp() noexcept;

    jackwarp(const jackwarp&) = delete;
    jackwarp& operator=(const jackwarp&) = delete;

    //------------------------------------------------------------------
    /// \brief Отправляет строку данных в подключённый сокет.
    ///
    /// Передача выполняется полностью (цикл send до отправки всех байт).
    /// \param data Строка для отправки.
    /// \throws JackNetworkError при ошибке send или отсутствии соединения.
    jackwarp& operator<<(const std::string&);

    //------------------------------------------------------------------
    /// \brief Читает данные из сокета в строку.
    ///
    /// Считывает максимум 1023 байта за раз.
    /// Если соединение закрыто, возвращает пустую строку.
    /// \throws JackNetworkError при ошибке recv().
    jackwarp& operator>>(std::string& out);
};


//======================================================================
// class jackhost
//----------------------------------------------------------------------
/// \brief Серверная сторона TCP-соединения.
///
/// Слушает указанный порт, принимает одно входящее соединение
/// и обеспечивает обмен данными через базовый интерфейс jackwarp.
///
class jackhost : public jackwarp {
    sockaddr_in host_address{};
public:
    explicit jackhost(); // пустой инициализатор

    /// \brief Создаёт серверный сокет и начинает прослушивание порта.
    ///
    /// \param port Порт для прослушивания.
    /// \throws JackNetworkError при ошибке bind() или listen().
    void jackhost_act(unsigned short);

    //------------------------------------------------------------------
    /// \brief Ожидает подключение клиента.
    ///
    /// После успешного вызова _jack становится активным соединением.
    /// \throws JackNetworkError при ошибке accept().
    void wait_jack();

    //------------------------------------------------------------------
    /// \brief Возвращает IP-адрес текущего хоста (IPv4).
    ///
    /// \return IP в формате "x.x.x.x".
    /// \throws JackNetworkError или JackAddressError при ошибках DNS/IP.
    std::string get_ip() const;

    //------------------------------------------------------------------
    /// \brief Возвращает invite-код, соответствующий IP-адресу хоста.
    ///
    /// Пример: 192.168.0.5 → "c0a80005".
    /// \throws JackAddressError при ошибке преобразования.
    std::string get_invite() const;
};

std::string get_invite(std::string);


//======================================================================
// class jack
//----------------------------------------------------------------------
/// \brief Клиентская сторона TCP-соединения.
///
/// Подключается к серверу (jackhost) по IP-адресу или invite-коду.
///
class jack : public jackwarp {
    sockaddr_in host_address{};
public:
    jack(); // пустой инициализатор

    /// \brief Создаёт подключение к указанному хосту и порту.
    ///
    /// \param host IPv4-адрес в формате "x.x.x.x".
    /// \param port Целевой порт.
    /// \throws JackAddressError при ошибке адресации.
    /// \throws JackNetworkError при ошибке подключения.
    void jack_act(const std::string& host, unsigned short);

    //------------------------------------------------------------------
    /// \brief Преобразует invite-код в IPv4-адрес.
    ///
    /// Пример: "c0a80005" → "192.168.0.5".
    /// \throws JackInviteError при некорректном коде.
    /// \throws JackAddressError при сбое inet_ntop().
    static std::string invite_to_ip(const std::string& code);
};
