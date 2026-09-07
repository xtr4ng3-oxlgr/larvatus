// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#include "larvatus/logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace larvatus {

// Opens the log file at the given path, creating it if needed.
Logger::Logger(const std::string& log_path) : log_path_(log_path) {}

// Escapes control characters and quotes so log data cannot break JSON structure.
std::string Logger::escape_json(const std::string& raw) {
    std::string escaped;
    escaped.reserve(raw.size());
    for (unsigned char c : raw) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    escaped += buf;
                } else {
                    escaped += static_cast<char>(c);
                }
        }
    }
    return escaped;
}

// Appends a single event record as one JSON line, escaping unsafe characters.
void Logger::log_event(const EventRecord& record) {
    std::lock_guard<std::mutex> lock(write_mutex_);

    std::ofstream out(log_path_, std::ios::app);
    if (!out.is_open()) {
        return;
    }

    out << "{"
        << "\"timestamp\":\"" << escape_json(record.timestamp_utc) << "\","
        << "\"source_ip\":\"" << escape_json(record.source_ip) << "\","
        << "\"source_port\":" << record.source_port << ","
        << "\"service\":\"" << escape_json(record.service_name) << "\","
        << "\"target_port\":" << record.target_port << ","
        << "\"banner_sent\":\"" << escape_json(record.banner_sent) << "\","
        << "\"data_received\":\"" << escape_json(record.data_received) << "\""
        << "}\n";
}

// Returns the current UTC time formatted as an ISO-8601 string.
std::string current_utc_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm utc_tm{};
#ifdef _WIN32
    gmtime_s(&utc_tm, &now_time);
#else
    gmtime_r(&now_time, &utc_tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

}  // namespace larvatus
