#pragma once
#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atomic>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#define close closesocket
typedef SOCKET socket_t;
#define IOCTL ioctlsocket
#else
#define closesocket close
typedef int socket_t;
#define IOCTL ioctl
#endif

namespace kcp_bridge
{
    const std::vector<uint8_t> HELLO_BYTE = { 0x48, 0x65, 0x6C, 0x6C, 0x6F }; // "Hello"
    bool IsHelloByte(const std::vector<uint8_t>& data);
    extern socket_t Socket;
    extern int ListenPort;
    // Get the hello package time from the data
    // The first 5 bytes are the "Hello" bytes, and the rest is the time
    // The time is stored as a uint64_t
    uint64_t GetHelloPackageTime(const std::vector<uint8_t>& data);
    int SendDataWithUdp(const socket_t socket, const std::string& ip, int port, const std::vector<uint8_t>& data);
    int ReceiveDataWithUdp(const socket_t socket, std::vector<uint8_t>& data, int port, std::string& recvIp, int& recvPort);
}