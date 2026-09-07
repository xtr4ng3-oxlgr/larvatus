// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#include "larvatus/listener.hpp"

#include <chrono>
#include <iostream>

namespace larvatus {

// Returns true if the given IP is still under its connection budget for this minute.
bool RateLimiter::allow(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mutex_);

    long long now_epoch = std::chrono::duration_cast<std::chrono::seconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();

    auto window_it = window_start_epoch_.find(ip);
    if (window_it == window_start_epoch_.end() || now_epoch - window_it->second >= 60) {
        window_start_epoch_[ip] = now_epoch;
        counts_this_minute_[ip] = 1;
        return true;
    }

    int& count = counts_this_minute_[ip];
    if (count >= kMaxConnectionsPerIpPerMinute) {
        return false;
    }
    count += 1;
    return true;
}

// Runs a blocking accept loop on one port, dispatching each connection to a decoy service.
void run_decoy_listener(uint16_t port, ServiceKind kind, Logger& logger, RateLimiter& limiter) {
    socket_t listener_sock = create_listener(port, 32);
    if (listener_sock == kInvalidSocket) {
        std::cerr << "[larvatus] failed to bind port " << port << "\n";
        return;
    }

    std::cout << "[larvatus] listening on port " << port << " as " << service_name(kind) << "\n";

    while (true) {
        std::string peer_ip;
        uint16_t peer_port = 0;
        socket_t client_sock = accept_connection(listener_sock, peer_ip, peer_port);
        if (client_sock == kInvalidSocket) {
            continue;
        }

        if (!limiter.allow(peer_ip)) {
            close_socket(client_sock);
            continue;
        }

        std::string captured = run_service_interaction(kind, client_sock);
        close_socket(client_sock);

        EventRecord record;
        record.timestamp_utc = current_utc_timestamp();
        record.source_ip = peer_ip;
        record.source_port = peer_port;
        record.service_name = service_name(kind);
        record.target_port = port;
        record.banner_sent = service_banner(kind);
        record.data_received = captured;

        logger.log_event(record);
        std::cout << "[larvatus] " << record.timestamp_utc << " " << peer_ip << " -> "
                  << service_name(kind) << ":" << port << "\n";
    }
}

}  // namespace larvatus
