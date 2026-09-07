// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
constexpr socket_t kInvalidSocket = INVALID_SOCKET;
#else
using socket_t = int;
constexpr socket_t kInvalidSocket = -1;
#endif

namespace larvatus {

// Initializes the underlying socket subsystem on platforms that require it.
bool platform_init();

// Releases resources held by the socket subsystem on shutdown.
void platform_shutdown();

// Closes a socket handle in a platform-correct way.
void close_socket(socket_t sock);

// Creates and binds a listening TCP socket on the given port.
socket_t create_listener(uint16_t port, int backlog);

// Accepts a pending connection and returns the client socket and its address.
socket_t accept_connection(socket_t listener, std::string& out_peer_ip, uint16_t& out_peer_port);

// Sends a null-terminated string over the given socket.
bool send_text(socket_t sock, const std::string& text);

// Reads up to max_len bytes from the socket with a bounded timeout, returns what was received.
std::string recv_with_timeout(socket_t sock, size_t max_len, int timeout_seconds);

}  // namespace larvatus
