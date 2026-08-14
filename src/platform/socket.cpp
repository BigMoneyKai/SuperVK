#include "platform/socket.h"
#include "core/define/platform.h"

#include <cstring>

#ifdef SV_PLATFORM_WINDOWS
static b32 g_wsaInitialized = false;
#endif

Socket::~Socket() { close(); }

b32 Socket::create(i32 af, i32 type, i32 protocol) {
#ifdef SV_PLATFORM_WINDOWS
  if (!g_wsaInitialized) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      return false;
    }
    g_wsaInitialized = true;
  }
#endif
  m_socket = ::socket(af, type, protocol);
  return m_socket != INVALID_SOCKET;
}

b32 Socket::connect(const String &host, u16 port) {
  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

#ifdef SV_PLATFORM_WINDOWS
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
#else
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
#endif

  return ::connect(m_socket, reinterpret_cast<struct sockaddr *>(&addr),
                   sizeof(addr)) != SOCKET_ERROR;
}

b32 Socket::bind(u16 port) {
  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  return ::bind(m_socket, reinterpret_cast<struct sockaddr *>(&addr),
                sizeof(addr)) != SOCKET_ERROR;
}

b32 Socket::listen(i32 backlog) {
  return ::listen(m_socket, backlog) != SOCKET_ERROR;
}

b32 Socket::accept(Socket &client) {
#ifdef SV_PLATFORM_WINDOWS
  int addrLen = sizeof(struct sockaddr_in);
#else
  socklen_t addrLen = sizeof(struct sockaddr_in);
#endif
  struct sockaddr_in addr{};
  client.m_socket =
      ::accept(m_socket, reinterpret_cast<struct sockaddr *>(&addr), &addrLen);
  return client.m_socket != INVALID_SOCKET;
}

i32 Socket::send(const void *data, u64 size) {
#ifdef SV_PLATFORM_WINDOWS
  return ::send(m_socket, static_cast<const char *>(data),
                static_cast<int>(size), 0);
#else
  return static_cast<i32>(::send(m_socket, data, size, 0));
#endif
}

i32 Socket::recv(void *buf, u64 size) {
#ifdef SV_PLATFORM_WINDOWS
  return ::recv(m_socket, static_cast<char *>(buf), static_cast<int>(size), 0);
#else
  return static_cast<i32>(::recv(m_socket, buf, size, 0));
#endif
}

b32 Socket::close() {
  if (m_socket == INVALID_SOCKET)
    return true;
#ifdef SV_PLATFORM_WINDOWS
  closesocket(m_socket);
#else
  ::close(m_socket);
#endif
  m_socket = INVALID_SOCKET;
  return true;
}

b32 Socket::setNonBlocking(b32 enabled) {
#ifdef SV_PLATFORM_WINDOWS
  u_long mode = enabled ? 1 : 0;
  return ioctlsocket(m_socket, FIONBIO, &mode) == 0;
#else
  i32 flags = fcntl(m_socket, F_GETFL, 0);
  if (flags == -1)
    return false;
  flags = enabled ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  return fcntl(m_socket, F_SETFL, flags) != -1;
#endif
}

b32 Socket::isValid() const { return m_socket != INVALID_SOCKET; }
