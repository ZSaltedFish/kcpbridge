#pragma once
#include <vector>
#include <thread>
#include <map>
#include <mutex>

#include "kcp_object.h"
#include "connection.h"

namespace kcp_bridge
{
    struct ReceiveData
    {
        std::vector<uint8_t> data;
        int len;
        std::string ip;
        int port;
    };

    class KcpManager
    {
    private:
        uint64_t _nextId = 0;
        KcpManager();
        ~KcpManager();
        bool _isDisposed = false;

        std::thread _updateThread;
        std::thread _receiveThread;
        std::atomic<bool> _running{ true };
        socket_t _socket;
        std::map<uint64_t, KcpObject*> _kcpObjects;
        std::vector<ReceiveData> _receiveDatas;

        std::mutex _objectsMtx;
        std::mutex _mtx;

        void KcpObjectDisposeCallback(KcpObject* user);

        void Receive();
        void Update();
    public:
        static KcpManager& GetInstance()
        {
            static KcpManager instance;
            return instance;
        }

        KcpObject* CreateKcpObject(const std::string& ip, int port);
        KcpObject* GetKcpObject(uint64_t id);
        void Dispose();
    };
}