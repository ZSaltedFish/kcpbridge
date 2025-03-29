project(kcp_bridge)

add_library(${PROJECT_NAME} SHARED
    ${KCP_SOURCE}
    kcp_bridge.cpp
    connection/connection.cpp
    connection/connection.h
    connection/connection_live.h
    connection/connection_live.cpp
    connection/kcp_object.h
    connection/kcp_object.cpp
    connection/kcp_object_tools.h
    connection/kcp_object_tools.cpp

    Tools/tools.h
    Tools/tools.cpp
)