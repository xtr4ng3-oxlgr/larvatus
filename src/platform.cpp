// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#include "larvatus/platform.hpp"

#include <cstring>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace larvatus {

// Initializes the underlying socket subsystem on platforms that require it.
bool platform_init() {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0;
#else
    return true;
#endif
}

// Releases resources held by the socket subsystem on shutdown.
void platform_shutdown() {
#ifdef _WIN32
    WSACleanup();
#endif
}

// Closes a socket handle in a platform-correct way.
void close_socket(socket_t sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

// Creates and binds a listening TCP socket on the given port.
socket_t create_listener(uint16_t port, int backlog) {
    socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == kInvalidSocket) {
        return kInvalidSocket;
    }

    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        close_socket(sock);
        return kInvalidSocket;
    }

    if (listen(sock, backlog) != 0) {
        close_socket(sock);
        return kInvalidSocket;
    }

    return sock;
}

// Accepts a pending connection and returns the client socket and its address.
socket_t accept_connection(socket_t listener, std::string& out_peer_ip, uint16_t& out_peer_port) {
    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);

    socket_t client = accept(listener, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
    if (client == kInvalidSocket) {
        return kInvalidSocket;
    }

    char ip_buf[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_buf, sizeof(ip_buf));
    out_peer_ip = ip_buf;
    out_peer_port = ntohs(client_addr.sin_port);

    return client;
}

// Sends a null-terminated string over the given socket.
bool send_text(socket_t sock, const std::string& text) {
    size_t sent_total = 0;
    while (sent_total < text.size()) {
        int sent = send(sock, text.data() + sent_total, static_cast<int>(text.size() - sent_total), 0);
        if (sent <= 0) {
            return false;
        }
        sent_total += static_cast<size_t>(sent);
    }
    return true;
}

// Reads up to max_len bytes from the socket with a bounded timeout, returns what was received.
std::string recv_with_timeout(socket_t sock, size_t max_len, int timeout_seconds) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    timeval tv{};
    tv.tv_sec = timeout_seconds;
    tv.tv_usec = 0;

    int ready = select(static_cast<int>(sock) + 1, &read_fds, nullptr, nullptr, &tv);
    if (ready <= 0) {
        return "";
    }

    std::string buffer(max_len, '\0');
    int received = recv(sock, buffer.data(), static_cast<int>(max_len), 0);
    if (received <= 0) {
        return "";
    }

    buffer.resize(static_cast<size_t>(received));
    return buffer;
}

}  // namespace larvatus
