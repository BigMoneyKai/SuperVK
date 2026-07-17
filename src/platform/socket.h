#pragma once

#include "defines.h"
#include "core/container/string/string.hpp"

#ifdef SV_PLATFORM_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socklen_t = int;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    using SocketHandle = int;
    static constexpr SocketHandle INVALID_SOCKET = -1;
    static constexpr int SOCKET_ERROR = -1;
#endif

class Socket {
public:
    Socket() = default;
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    b32 create(i32 af = AF_INET, i32 type = SOCK_STREAM, i32 protocol = 0);
    b32 connect(const String& host, u16 port);
    b32 bind(u16 port);
    b32 listen(i32 backlog = 5);
    b32 accept(Socket& client);
    i32 send(const void* data, u64 size);
    i32 recv(void* buf, u64 size);
    b32 close();

    b32 setNonBlocking(b32 enabled);
    b32 isValid() const;

private:
#ifdef SV_PLATFORM_WINDOWS
    SOCKET m_socket{INVALID_SOCKET};
#else
    int m_socket{INVALID_SOCKET};
#endif
};
