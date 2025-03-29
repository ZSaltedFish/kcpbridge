#pragma once

#include "connection.h"
#include "connection_live.h"
#include "kcp_object_start_tools.h"

namespace kcp_bridge
{
    class KcpObject
    {
    private:
        Connection* _connection;
        ConnectionLive* _connectionLive;
        bool _isDisposed = false;
        bool _isConnected = false;
        socket_t _socketId;
        
        inline void ThrowIfDisposed() const { if (_isDisposed) throw std::runtime_error("KcpObject is disposed"); }
    public:
        KcpObject(const std::string& ip, int port);
        ~KcpObject();
        void Send(const std::vector<uint8_t>& data);

        inline void SendUdp(const std::vector<uint8_t>& data)
        {
            ThrowIfDisposed();
            _connection->SendUdpWithoutKcp(data);
        }
        void Dispose();
        
        void Update();
        void Receive();
        bool ReceiveHelloCheck(std::vector<uint8_t>& data);

        inline void SetHeartBeatTimeMs(uint64_t time) { _connectionLive->SetHeartBeatTimeMs(time); }
        inline void SetDisconnectTimeoutMs(uint64_t timeout) { _connectionLive->SetDisconnectTimeoutMs(timeout); }
        inline socket_t GetSocketId() const { return _socketId; }
        inline int SendConnectPackage() { return _connection->SendUdpWithoutKcp(HELLO_BYTE); }
    };
}