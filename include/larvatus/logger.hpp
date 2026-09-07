// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#pragma once

#include <mutex>
#include <string>

namespace larvatus {

// Represents a single logged interaction with a decoy service.
struct EventRecord {
    std::string timestamp_utc;
    std::string source_ip;
    uint16_t source_port;
    std::string service_name;
    uint16_t target_port;
    std::string banner_sent;
    std::string data_received;
};

// Writes decoy interaction events to a JSON Lines file in a thread-safe manner.
class Logger {
public:
    // Opens the log file at the given path, creating it if needed.
    explicit Logger(const std::string& log_path);

    // Appends a single event record as one JSON line, escaping unsafe characters.
    void log_event(const EventRecord& record);

private:
    std::string log_path_;
    std::mutex write_mutex_;

    // Escapes control characters and quotes so log data cannot break JSON structure.
    static std::string escape_json(const std::string& raw);
};

// Returns the current UTC time formatted as an ISO-8601 string.
std::string current_utc_timestamp();

}  // namespace larvatus
