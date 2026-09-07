// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#pragma once

#include <cstdint>
#include <map>
#include <mutex>
#include <string>

#include "larvatus/logger.hpp"
#include "larvatus/service_profile.hpp"

namespace larvatus {

// Bounds how many connections a single source IP may open per rolling minute.
constexpr int kMaxConnectionsPerIpPerMinute = 20;

// Tracks recent connection counts per source IP to throttle abusive callers.
class RateLimiter {
public:
    // Returns true if the given IP is still under its connection budget for this minute.
    bool allow(const std::string& ip);

private:
    std::mutex mutex_;
    std::map<std::string, int> counts_this_minute_;
    std::map<std::string, long long> window_start_epoch_;
};

// Runs a blocking accept loop on one port, dispatching each connection to a decoy service.
void run_decoy_listener(uint16_t port, ServiceKind kind, Logger& logger, RateLimiter& limiter);

}  // namespace larvatus
