project(kcp_bridge)

add_library(${PROJECT_NAME} SHARED
    ${KCP_SOURCE}
    kcp_bridge.cpp
)