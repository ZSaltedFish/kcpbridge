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
    };
}