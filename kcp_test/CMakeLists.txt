project(kcp_test LANGUAGES CXX)

add_executable(
    ${PROJECT_NAME}
    main.cpp
 "Server/server.h")

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        kcp_bridge
)