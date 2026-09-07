// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#include "larvatus/service_profile.hpp"

namespace larvatus {

// Converts a service kind to its display name for logging.
std::string service_name(ServiceKind kind) {
    switch (kind) {
        case ServiceKind::Ssh: return "ssh";
        case ServiceKind::Telnet: return "telnet";
        case ServiceKind::Rdp: return "rdp";
        case ServiceKind::Ftp: return "ftp";
    }
    return "unknown";
}

// Returns the banner text a real instance of this service would present.
std::string service_banner(ServiceKind kind) {
    switch (kind) {
        case ServiceKind::Ssh: return "SSH-2.0-OpenSSH_9.6p1 Ubuntu-3ubuntu1\r\n";
        case ServiceKind::Telnet: return "\r\nUbuntu 22.04 LTS\r\nlogin: ";
        case ServiceKind::Rdp: return "";
        case ServiceKind::Ftp: return "220 (vsFTPd 3.0.5)\r\n";
    }
    return "";
}

// Runs a decoy Telnet login prompt and returns whatever credentials were typed.
static std::string interact_telnet(socket_t client_sock) {
    send_text(client_sock, service_banner(ServiceKind::Telnet));
    std::string username = recv_with_timeout(client_sock, 256, 15);

    send_text(client_sock, "Password: ");
    std::string password = recv_with_timeout(client_sock, 256, 15);

    send_text(client_sock, "\r\nLogin incorrect\r\n");
    return "username=" + username + " password=" + password;
}

// Runs a decoy SSH version exchange and returns the client's identification string.
static std::string interact_ssh(socket_t client_sock) {
    send_text(client_sock, service_banner(ServiceKind::Ssh));
    std::string client_ident = recv_with_timeout(client_sock, 256, 10);
    return "client_ident=" + client_ident;
}

// Runs a decoy FTP login prompt and returns whatever credentials were typed.
static std::string interact_ftp(socket_t client_sock) {
    send_text(client_sock, service_banner(ServiceKind::Ftp));
    std::string user_line = recv_with_timeout(client_sock, 256, 15);

    send_text(client_sock, "331 Please specify the password.\r\n");
    std::string pass_line = recv_with_timeout(client_sock, 256, 15);

    send_text(client_sock, "530 Login incorrect.\r\n");
    return "user_line=" + user_line + " pass_line=" + pass_line;
}

// Accepts the raw connection bytes for RDP without emulating the binary handshake.
static std::string interact_rdp(socket_t client_sock) {
    std::string initial_bytes = recv_with_timeout(client_sock, 256, 10);
    return "initial_bytes_len=" + std::to_string(initial_bytes.size());
}

// Runs one full decoy interaction on an already-accepted client socket and returns captured input.
std::string run_service_interaction(ServiceKind kind, socket_t client_sock) {
    switch (kind) {
        case ServiceKind::Ssh: return interact_ssh(client_sock);
        case ServiceKind::Telnet: return interact_telnet(client_sock);
        case ServiceKind::Ftp: return interact_ftp(client_sock);
        case ServiceKind::Rdp: return interact_rdp(client_sock);
    }
    return "";
}

}  // namespace larvatus
