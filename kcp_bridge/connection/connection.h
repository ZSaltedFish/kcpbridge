#pragma once
#include <ikcp.h>
#include <vector>
#include <string>

namespace kcp_bridge
{
    class Connection
    {
    private:
        static int UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user);
        ikcpcb* _kcp;

        std::string _ip;
        int _port;
    public:
        Connection& operator=(const Connection&) = delete;
        Connection(const Connection&) = delete;
        Connection(const std::string ip, int port);
        ~Connection();

        inline ikcpcb* GetKcp() const { return _kcp; }
        inline void Input(const std::vector<uint8_t>& data) { ikcp_input(_kcp, (const char*)data.data(), data.size()); }
        void Send(const std::vector<uint8_t>& data) const;
    };
}