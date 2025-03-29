#pragma once
#include <vector>

namespace kcp_bridge
{
    const std::vector<uint8_t> HELLO_BYTE = { 0x48, 0x65, 0x6C, 0x6C, 0x6F }; // "Hello"
    bool IsHelloByte(const std::vector<uint8_t>& data);

    // Get the hello package time from the data
    // The first 5 bytes are the "Hello" bytes, and the rest is the time
    // The time is stored as a uint64_t
    uint64_t GetHelloPackageTime(const std::vector<uint8_t>& data);
    
}