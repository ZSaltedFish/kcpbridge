#include <vector>
#include <thread>
#include <map>
#include "connection.h"

namespace kcp_bridge
{
    class KcpManager
    {
    private:
        KcpManager() = default;
        ~KcpManager() = default;

        std::thread _updateThread;
        std::thread _receiveThread;
        std::atomic<bool> _running{ true };
    public:
        static KcpManager& GetInstance()
        {
            static KcpManager instance;
            return instance;
        }
    };
}