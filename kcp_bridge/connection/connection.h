#include <ikcp.h>
#include <vector>
#include <string>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
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
    class Connection
    {
    private:
        static int UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user);

        socket_t _socket;
        ikcpcb* _kcp;

        std::string _ip;
        int _port;
    public:
        Connection& operator=(const Connection&) = delete;
        Connection(const Connection&) = delete;
        Connection(const std::string ip, int port);
        ~Connection();
        socket_t GetSocket() const { return _socket; }

        inline ikcpcb* GetKcp() const { return _kcp; }
        void Send(const std::vector<uint8_t>& data) const;
        int SendUdpWithoutKcp(const std::vector<uint8_t>& data) const;
    };
}