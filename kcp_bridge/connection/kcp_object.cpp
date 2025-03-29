#include <iostream>
#include "kcp_object.h"
#include "../Tools/tools.h"

namespace kcp_bridge
{
    KcpObject::KcpObject(uint64_t id, const std::string& ip, int port) : _ip(ip), _port(port), _kcpObjectId(id)
    {
        _connection = new Connection(ip, port);
        _connectionLive = new ConnectionLive();

        _disposeReason = KcpDisposeReason::Error;
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

    int KcpObject::SendUdp(const std::vector<uint8_t>& data)
    {
		ThrowIfDisposed();
		return SendDataWithUdp(Socket, _ip, _port, data);
	}

    bool KcpObject::ReceiveHelloCheck(const std::vector<uint8_t>& data)
    {
        uint64_t helloTime = GetHelloPackageTime(data);
        if (helloTime == 0) return false;
        _connectionLive->UpdateLastReceiveTime();
        _isConnected = true;
        return true;
    }

    bool KcpObject::ReceiveHeartBeatCheck(const std::vector<uint8_t>& data)
    {
        uint64_t heartBeatTime = GetHelloPackageTime(data);
        if (heartBeatTime == 0) return false;
        _connectionLive->UpdateLastReceiveTime();
        return true;
    }

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
            SendUdp(heartBeatPackage);
            _connectionLive->UpdateLastSendTime();
        }

        if (_connectionLive->IsDisconnected())
        {
            std::cerr << "KcpObject is disconnected." << std::endl;
            Dispose(KcpDisposeReason::Timeout);
        }
    }

    void KcpObject::UpdateKcp()
    {
        uint64_t current = GetTickCount64();
        if (current > _kcpUpdateCount)
        {
            ikcp_update(_connection->GetKcp(), current);
            _kcpUpdateCount = ikcp_check(_connection->GetKcp(), current);
		}
    }

    void KcpObject::Receive(const std::vector<uint8_t>& data)
    {
        if (ReceiveHelloCheck(data)) return;
        if (ReceiveHeartBeatCheck(data)) return;

        _connection->Input(data);
    }
}