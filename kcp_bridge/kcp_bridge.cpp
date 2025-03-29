#include <iostream>

#include "connection/kcp_object_tools.h"
extern "C"
{
    void kcp_say_hello()
    {
        std::cout << "Hello from KCP!" << std::endl;
    }

    void SetListenPort(int port)
    {
        kcp_bridge::ListenPort = port;
    }
}