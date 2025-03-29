#include <iostream>
#include "kcp_object.h"

namespace kcp_bridge
{
    KcpObject::KcpObject(const std::string& ip, int port)
    {
        _connection = new Connection(ip, port);
        _connectionLive = new ConnectionLive();
        _socketId = _connection->GetSocket();

        _isDisposed = false;
    }

    KcpObject::~KcpObject()
    {
        Dispose(KcpDisposeReason::Error);
    }
    
    void KcpObject::Dispose(KcpDisposeReason reason)
    {
        if (_isDisposed) return;
        _disposeReason = reason;
        if (_connection)
        {
            delete _connection;
            _connection = nullptr;
        }

        if (_connectionLive)
        {
            delete _connectionLive;
            _connectionLive = nullptr;
        }
        _isConnected = false;
        _isDisposed = true;

        if (_disposeCallback)
        {
            _disposeCallback(*this);
        }
    }

    void KcpObject::Send(const std::vector<uint8_t>& data)
    {
        ThrowIfDisposed();
        if (_isConnected)
        {
            _connection->Send(data);
        }
        else
        {
            std::cerr << "KcpObject is not connected." << std::endl;
        }
    }

    bool KcpObject::ReceiveHelloCheck(const std::vector<uint8_t>& data)
    {
        uint64_t helloTime = GetHelloPackageTime(data);
        if (helloTime == 0) return false;
        _connectionLive->UpdateLastReceiveTime();
        _isConnected = true;
        return true;
    }

    bool KcpObject::ReceiveHeartBeatCheck()

    void KcpObject::Update()
    {
        if (_isDisposed) return;
        UpdateLive();
        UpdateKcp();
    }

    void KcpObject::UpdateLive()
    {
        if (!_isConnected) return;
        if (_connectionLive->IsHeartBeat())
        {
            auto heartBeatPackage = _connectionLive->GetHeartBeatPackage();
            _connection->SendUdpWithoutKcp(heartBeatPackage);
            _connectionLive->UpdateLastSendTime();
        }

        if (_connectionLive->IsDisconnected())
        {
            std::cerr << "KcpObject is disconnected." << std::endl;
            Dispose(KcpDisposeReason::Timeout);
        }
    }
}