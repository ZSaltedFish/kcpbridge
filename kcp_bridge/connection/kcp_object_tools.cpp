#include "kcp_object_tools.h"
#include "../Tools/tools.h"

namespace kcp_bridge
{
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
}