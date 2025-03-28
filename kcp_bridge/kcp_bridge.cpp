#include <iostream>

extern "C"
{
    void kcp_say_hello()
    {
        std::cout << "Hello from KCP!" << std::endl;
    }
}