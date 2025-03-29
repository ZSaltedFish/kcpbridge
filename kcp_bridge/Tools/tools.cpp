#include "tools.h"
#include <chrono>

namespace kcp_bridge
{
    uint64_t GetTickCount64()
    {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return static_cast<uint64_t>(ms);
    }
}