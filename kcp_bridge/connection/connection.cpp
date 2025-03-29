#include <chrono>
#include <iostream>
#include "connection.h"
#include "../Tools/tools.h"

namespace kcp_bridge
{
    Connection::Connection(const std::string ip, int port)
    {
        _ip = ip;
        _port = port;
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            throw std::runtime_error("Failed to initialize Winsock");
        }
#endif

        _socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket < 0)
        {
            throw std::runtime_error("Failed to create socket");
        }

        u_long mode = 1;
        if (IOCTL(_socket, FIONBIO, &mode) < 0)
        {
            throw std::runtime_error("Failed to set socket to non-blocking mode");
        }

        _kcp = ikcp_create(0x11223344, this);
        ikcp_setoutput(_kcp, &Connection::UdpOutput);
        ikcp_nodelay(_kcp, 1, 10, 2, 1);
        ikcp_setmtu(_kcp, 1400);
    }

    Connection::~Connection()
    {
        if (_socket != -1)
        {
            closesocket(_socket);
            ikcp_release(_kcp);
#ifdef _WIN32
            WSACleanup();
#endif
        }
    }

    int Connection::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
    {
        auto kcpConnection = (Connection*)user;
        auto socket = kcpConnection->_socket;
        char* ip = (char*)kcpConnection->_ip.c_str();
        int port = kcpConnection->_port;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        int ret = sendto(socket, buf, len, 0, (sockaddr*)&addr, sizeof(addr));
        if (ret < 0)
        {
            std::cerr << "sendto error: " << ret << std::endl;
            return -1;
        }
        return 0;
    }

    void Connection::Send(const std::vector<uint8_t>& data) const
    {
        if (data.empty()) return;

        auto len = data.size();
        auto ret = ikcp_send(_kcp, (const char*)data.data(), len);
        if (ret < 0)
        {
            std::cerr << "ikcp_send error: " << ret << std::endl;
            return;
        }
    }

    int Connection::SendUdpWithoutKcp(const std::vector<uint8_t>& data) const
    {
        if (data.empty()) return 0;
        
        auto ip = (char*)_ip.c_str();
        auto port = _port;
        auto len = data.size();
        auto buf = (char*)data.data();

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        int ret = sendto(_socket, buf, len, 0, (sockaddr*)&addr, sizeof(addr));
        if (ret < 0)
        {
            std::cerr << "sendto error: " << ret << std::endl;
            return -1;
        }
        return 0;
    }
}