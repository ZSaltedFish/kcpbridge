#pragma once
#include "../Tools/tools.h"

namespace kcp_bridge
{
    class ConnectionLive
    {
    private:
        uint64_t _disconnectTimeoutMs = 30000;
        uint64_t _heartBeatTimeMs = 5000;
        uint64_t _lastSendTime = 0;
        uint64_t _lastReceiveTime = 0;
    public:
        const std::vector<uint8_t> HEART_BEAT_BYTE = { 0x48, 0x65, 0x61, 0x72, 0x74, 0x42, 0x65, 0x61, 0x74 }; // "HeartBeat"

        ConnectionLive() = default;
        ~ConnectionLive() = default;

        inline uint64_t GetDisconnectTimeoutMs() const { return _disconnectTimeoutMs; }
        inline void SetDisconnectTimeoutMs(uint64_t timeout) { _disconnectTimeoutMs = timeout; }
        inline uint64_t GetHeartBeatTimeMs() const { return _heartBeatTimeMs; }
        inline void SetHeartBeatTimeMs(uint64_t time) { _heartBeatTimeMs = time; }

        inline bool IsHeartBeat() const { return (GetTickCount64() - _lastSendTime) >= _heartBeatTimeMs; }
        inline bool IsDisconnected() const { return (GetTickCount64() - _lastReceiveTime) >= _disconnectTimeoutMs; }
        inline void UpdateLastSendTime() { _lastSendTime = GetTickCount64(); }
        inline void UpdateLastReceiveTime() { _lastReceiveTime = GetTickCount64(); }
        
        static std::vector<uint8_t> GetHeartBeatPackage()
        {
            static std::vector<uint8_t> heartBeatPackage = { 0x48, 0x65, 0x61, 0x72, 0x74, 0x42, 0x65, 0x61, 0x74 }; // "HeartBeat"
            uint64_t currentTime = GetTickCount64();
            std::vector<uint8_t> timeBytes = GetBytes(currentTime);
            heartBeatPackage.insert(heartBeatPackage.end(), timeBytes.begin(), timeBytes.end());
            return heartBeatPackage;
        }

        static uint64_t IsHeartBeatPackage(const std::vector<uint8_t>& package)
        {
            static std::vector<uint8_t> heartBeatPackage = { 0x48, 0x65, 0x61, 0x72, 0x74, 0x42, 0x65, 0x61, 0x74 }; // "HeartBeat"
            if (package.size() <= heartBeatPackage.size()) return 0;

            for (int i = 0; i < heartBeatPackage.size(); ++i)
            {
                if (package[i] != heartBeatPackage[i]) return 0;
            }

            return ToUInt64(package, heartBeatPackage.size());
        }
    };
}