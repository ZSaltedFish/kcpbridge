#include <iostream>
#include "kcp_manager.h"

namespace kcp_bridge
{
    void KcpManager::KcpObjectDisposeCallback(KcpObject* user)
    {
        if (user == nullptr) return;
		auto kcpManager = (KcpManager*)user;
        auto id = user->GetKcpObjectId();
        std::cout << "KcpObject " << id << " disposed. Reason: " << (int)user->GetDisposeReason() << std::endl;
	}

	KcpManager::KcpManager()
	{
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

        Socket = _socket;

        u_long mode = 1;
        if (IOCTL(_socket, FIONBIO, &mode) < 0)
        {
            throw std::runtime_error("Failed to set socket to non-blocking mode");
        }
	}

    KcpManager::~KcpManager()
    {
        if (_socket != -1)
        {
            Dispose();
			closesocket(_socket);
#ifdef _WIN32
            WSACleanup();
#endif
        }
    }

    void KcpManager::Update()
    {
        std::vector<KcpObject*> objects;
        std::vector<ReceiveData> receiveDatas;
        while (_running)
        {
            {
                receiveDatas.clear();

                std::lock_guard<std::mutex> lock(_mtx);
                for (auto& recvData : _receiveDatas)
                {
                    receiveDatas.push_back(recvData);
                }
            }

            {
                std::lock_guard<std::mutex> lock(_objectsMtx);
                objects.clear();
                for (auto& [id, kcpObject] : _kcpObjects)
                {
                    objects.push_back(kcpObject);
                }
            }

            for (auto kcpObject : objects)
            {
                if (!_running) break;

                for (auto& receiveData : receiveDatas)
                {
                    if (receiveData.ip == kcpObject->GetIp() && receiveData.port == kcpObject->GetPort())
                    {
                        kcpObject->Receive(receiveData.data);
                    }
                }

                kcpObject->Update();
            }

            for (auto& kcpObject : objects)
            {
                if (!kcpObject->IsDisposed()) continue;
                std::lock_guard<std::mutex> lock(_objectsMtx);
                _kcpObjects.erase(kcpObject->GetKcpObjectId());
                delete kcpObject;
            }

            for (auto& receiveData : receiveDatas)
            {
                if (!IsHelloByte(receiveData.data)) continue;
                auto kcpObject = CreateKcpObject(receiveData.ip, receiveData.port);
                kcpObject->Receive(receiveData.data);
                kcpObject->SendConnectPackage();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void KcpManager::Receive()
    {
        while (_running)
        {
            ReceiveData receiveData;
			receiveData.len = ReceiveDataWithUdp(_socket, receiveData.data, ListenPort, receiveData.ip, receiveData.port);
            if (receiveData.len > 0)
            {
				std::lock_guard<std::mutex> lock(_mtx);
				_receiveDatas.push_back(receiveData);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void KcpManager::Dispose()
    {
        if (_isDisposed) return;
        _isDisposed = true;
        _running = false;
        for (auto& [id, kcpObject] : _kcpObjects)
        {
			delete kcpObject;
		}
    }
}