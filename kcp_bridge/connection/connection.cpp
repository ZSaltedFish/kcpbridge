#include <chrono>
#include <iostream>
#include "connection.h"
#include "../Tools/tools.h"
#include "kcp_object_tools.h"

namespace kcp_bridge
{
    Connection::Connection(const std::string ip, int port)
    {
        _ip = ip;
        _port = port;

        _kcp = ikcp_create(0x11223344, this);
        ikcp_setoutput(_kcp, &Connection::UdpOutput);
        ikcp_nodelay(_kcp, 1, 10, 2, 1);
        ikcp_setmtu(_kcp, 1400);
    }

    Connection::~Connection()
    {
        if (!_kcp) return;
        ikcp_release(_kcp);
    }

    int Connection::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
    {
        auto kcpConnection = (Connection*)user;
        auto socket = Socket;
        char* ip = (char*)kcpConnection->_ip.c_str();
        int port = kcpConnection->_port;

        return SendDataWithUdp(socket, ip, port, std::vector<uint8_t>(buf, buf + len));
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
}