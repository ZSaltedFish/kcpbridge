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

    std::vector<uint8_t> GetBytes(double value)
    {
        std::vector<uint8_t> bytes(sizeof(double));
        std::memcpy(bytes.data(), &value, sizeof(double));
        return bytes;
    }
    std::vector<uint8_t> GetBytes(int value)
    {
        std::vector<uint8_t> bytes(sizeof(int));
        std::memcpy(bytes.data(), &value, sizeof(int));
        return bytes;
    }
    std::vector<uint8_t> GetBytes(uint32_t value)
    {
        std::vector<uint8_t> bytes(sizeof(uint32_t));
        std::memcpy(bytes.data(), &value, sizeof(uint32_t));
        return bytes;
    }
    std::vector<uint8_t> GetBytes(uint64_t value)
    {
        std::vector<uint8_t> bytes(sizeof(uint64_t));
        std::memcpy(bytes.data(), &value, sizeof(uint64_t));
        return bytes;
    }
    std::vector<uint8_t> GetBytes(float value)
    {
        std::vector<uint8_t> bytes(sizeof(float));
        std::memcpy(bytes.data(), &value, sizeof(float));
        return bytes;
    }
    double ToDouble(const std::vector<uint8_t>& data, int offset)
    {
        double value;
        std::memcpy(&value, data.data() + offset, sizeof(double));
        return value;
    }
    int ToInt(const std::vector<uint8_t>& data, int offset)
    {
        int value;
        std::memcpy(&value, data.data() + offset, sizeof(int));
        return value;
    }
    uint32_t ToUInt32(const std::vector<uint8_t>& data, int offset)
    {
        uint32_t value;
        std::memcpy(&value, data.data() + offset, sizeof(uint32_t));
        return value;
    }
    uint64_t ToUInt64(const std::vector<uint8_t>& data, int offset)
    {
        uint64_t value;
        std::memcpy(&value, data.data() + offset, sizeof(uint64_t));
        return value;
    }
    float ToFloat(const std::vector<uint8_t>& data, int offset)
    {
        float value;
        std::memcpy(&value, data.data() + offset, sizeof(float));
        return value;
    }
}