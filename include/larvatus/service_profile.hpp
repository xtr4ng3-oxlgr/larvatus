// by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
#pragma once

#include <string>

#include "larvatus/platform.hpp"

namespace larvatus {

// Identifies the type of decoy service a listener emulates.
enum class ServiceKind { Ssh, Telnet, Rdp, Ftp };

// Converts a service kind to its display name for logging.
std::string service_name(ServiceKind kind);

// Returns the banner text a real instance of this service would present.
std::string service_banner(ServiceKind kind);

// Runs one full decoy interaction on an already-accepted client socket and returns captured input.
std::string run_service_interaction(ServiceKind kind, socket_t client_sock);

}  // namespace larvatus
