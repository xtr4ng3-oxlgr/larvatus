// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "larvatus/listener.hpp"
#include "larvatus/logger.hpp"
#include "larvatus/platform.hpp"
#include "larvatus/service_profile.hpp"

namespace {

// Maps a config file service label to its ServiceKind, returns false if unrecognized.
bool parse_service_kind(const std::string& label, larvatus::ServiceKind& out_kind) {
    if (label == "ssh") { out_kind = larvatus::ServiceKind::Ssh; return true; }
    if (label == "telnet") { out_kind = larvatus::ServiceKind::Telnet; return true; }
    if (label == "rdp") { out_kind = larvatus::ServiceKind::Rdp; return true; }
    if (label == "ftp") { out_kind = larvatus::ServiceKind::Ftp; return true; }
    return false;
}

// Represents one "service,port" line parsed from the config file.
struct DecoyEntry {
    larvatus::ServiceKind kind;
    uint16_t port;
};

// Reads the config file and returns the list of decoy services to run.
std::vector<DecoyEntry> load_config(const std::string& path) {
    std::vector<DecoyEntry> entries;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[larvatus] could not open config file: " << path << "\n";
        return entries;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream ss(line);
        std::string service_label;
        std::string port_text;
        if (!std::getline(ss, service_label, ',') || !std::getline(ss, port_text)) {
            continue;
        }

        larvatus::ServiceKind kind;
        if (!parse_service_kind(service_label, kind)) {
            std::cerr << "[larvatus] unknown service in config: " << service_label << "\n";
            continue;
        }

        entries.push_back({kind, static_cast<uint16_t>(std::stoi(port_text))});
    }

    return entries;
}

}  // namespace

// Loads the config, starts one listener thread per decoy service, and waits forever.
int main(int argc, char** argv) {
    if (!larvatus::platform_init()) {
        std::cerr << "[larvatus] failed to initialize socket subsystem\n";
        return 1;
    }

    std::string config_path = argc > 1 ? argv[1] : "config/larvatus.conf";
    std::vector<DecoyEntry> entries = load_config(config_path);

    if (entries.empty()) {
        std::cerr << "[larvatus] no decoy services configured, exiting\n";
        larvatus::platform_shutdown();
        return 1;
    }

    larvatus::Logger logger("larvatus_events.jsonl");
    larvatus::RateLimiter limiter;

    std::vector<std::thread> threads;
    for (const DecoyEntry& entry : entries) {
        threads.emplace_back(larvatus::run_decoy_listener, entry.port, entry.kind, std::ref(logger),
                              std::ref(limiter));
    }

    for (std::thread& t : threads) {
        t.join();
    }

    larvatus::platform_shutdown();
    return 0;
}
