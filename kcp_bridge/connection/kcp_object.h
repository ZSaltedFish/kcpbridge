#pragma once

#include <functional>
#include "connection.h"
#include "connection_live.h"
#include "kcp_object_tools.h"

namespace kcp_bridge
{
    enum class KcpDisposeReason
    {
        TargetDisconnect,
        Timeout,
        Error
    };

    class KcpObject
    {
    private:
        Connection* _connection;
        ConnectionLive* _connectionLive;
        bool _isDisposed = false;
        bool _isConnected = false;
        KcpDisposeReason _disposeReason;
        uint64_t _kcpUpdateCount = 0;

        std::function<void(KcpObject&)> _disposeCallback;
        inline void ThrowIfDisposed() const { if (_isDisposed) throw std::runtime_error("KcpObject is disposed"); }
        void UpdateLive();
        void UpdateKcp();
        bool ReceiveHelloCheck(const std::vector<uint8_t>& data);
        bool ReceiveHeartBeatCheck(const std::vector<uint8_t>& data);
        std::string _ip;
        int _port;
        uint64_t _kcpObjectId;
    public:
        KcpObject(uint64_t id, const std::string& ip, int port);
        ~KcpObject();
        void Send(const std::vector<uint8_t>& data);

        int SendUdp(const std::vector<uint8_t>& data);
        void Dispose(KcpDisposeReason reason);
        
        void Update();
        void Receive(const std::vector<uint8_t>& data);

        inline void SetHeartBeatTimeMs(uint64_t time) { _connectionLive->SetHeartBeatTimeMs(time); }
        inline void SetDisconnectTimeoutMs(uint64_t timeout) { _connectionLive->SetDisconnectTimeoutMs(timeout); }
        inline int SendConnectPackage() { return SendUdp(HELLO_BYTE); }
        inline KcpDisposeReason GetDisposeReason() const { return _disposeReason; }
        inline std::string GetIp() const { return _ip; }
        inline int GetPort() const { return _port; }
        inline uint64_t GetKcpObjectId() const { return _kcpObjectId; }
        inline bool IsConnected() const { return _isConnected; }
        inline bool IsDisposed() const { return _isDisposed; }
    };
}