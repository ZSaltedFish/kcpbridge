#pragma once

#include <cstdint>
#include <vector>

namespace kcp_bridge
{
    uint64_t GetTickCount64();
    std::vector<uint8_t> GetBytes(double value);
    std::vector<uint8_t> GetBytes(int value);
    std::vector<uint8_t> GetBytes(uint32_t value);
    std::vector<uint8_t> GetBytes(uint64_t value);
    std::vector<uint8_t> GetBytes(float value);

    double ToDouble(const std::vector<uint8_t>& data, int offset = 0);
    int ToInt(const std::vector<uint8_t>& data, int offset = 0);
    uint32_t ToUInt32(const std::vector<uint8_t>& data, int offset = 0);
    uint64_t ToUInt64(const std::vector<uint8_t>& data, int offset = 0);
    float ToFloat(const std::vector<uint8_t>& data, int offset = 0);
}