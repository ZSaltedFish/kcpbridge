#include <iostream>

#include "kcp_object_tools.h"
#include "../Tools/tools.h"

namespace kcp_bridge
{
    socket_t Socket = -1;
    int ListenPort = 0;
    bool IsHelloByte(const std::vector<uint8_t>& data)
    {
        if (data.size() < HELLO_BYTE.size()) return false;
        for (size_t i = 0; i < HELLO_BYTE.size(); ++i)
        {
            if (data[i] != HELLO_BYTE[i]) return false;
        }
        return true;
    }

    uint64_t GetHelloPackageTime(const std::vector<uint8_t>& data)
    {
        if (!IsHelloByte(data)) return 0;
        return ToUInt64(data, HELLO_BYTE.size());
    }

    int SendDataWithUdp(const socket_t socket, const std::string& ip, int port, const std::vector<uint8_t>& data)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        return sendto(socket, (const char*)data.data(), data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
    }

    int ReceiveDataWithUdp(const socket_t socket, std::vector<uint8_t>& data, int port, std::string& recvIp, int& recvPort)
    {
        sockaddr_in addr;
		socklen_t addrLen = sizeof(addr);
		data.resize(1024);
		int ret = recvfrom(socket, (char*)data.data(), data.size(), 0, (struct sockaddr*)&addr, &addrLen);
        if (ret < 0)
        {
			std::cerr << "recvfrom error: " << ret << std::endl;
			return -1;
		}
		data.resize(ret);

        recvIp = inet_ntoa(addr.sin_addr);
        recvPort = ntohs(addr.sin_port);
		return ret;
    }
}