cmake_minimum_required(VERSION 3.16)
project(sik-bomberman)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

# See make commands.
set (CMAKE_VERBOSE_MAKEFILE ON)

# Settings GCC 11.2 compiler
cmake_host_system_information(RESULT _host_name QUERY HOSTNAME)

if (${_host_name} STREQUAL "students")
    message ("Setting gcc11.2 (remember about: export LD_LIBRARY_PATH=/opt/gcc-11.2/lib64)")
    set (LD_LIBRARY_PATH "/opt/gcc-11.2/lib64")
    set (CMAKE_CXX_COMPILER "/opt/gcc-11.2/bin/g++-11.2")
else()
    message ("Setting gcc11.2 (remember about: export LD_LIBRARY_PATH=/opt/gcc-11.2/lib64)")
    set (LD_LIBRARY_PATH "/home/amwms/.GCC-11.2/lib64")
    set (CMAKE_CXX_COMPILER "/home/amwms/.GCC-11.2/bin/g++")
endif ()

# Set flags
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Werror -std=gnu++20 -pthread")
set (CMAKE_CXX_FLAGS_DEBUG "-g")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

# Add executable client
set (CLIENT_EXECUTABLE robots-client)
add_executable (${CLIENT_EXECUTABLE}
        src/client/main.cpp
        src/client/arguments.cpp
        src/utils/serializer.cpp src/utils/serializer.h
        src/utils/messages/DrawMessage.cpp src/utils/messages/DrawMessage.h
        src/utils/utils.cpp src/utils/utils.h
        src/utils/messages/InputMessage.cpp src/utils/messages/InputMessage.h
        src/utils/deserialization.cpp src/utils/deserialization.h
        src/utils/event.cpp src/utils/event.h
        src/client/GuiConnector.cpp src/client/GuiConnector.h
        src/utils/messages/ServerMessage.cpp src/utils/messages/ServerMessage.h
        src/client/ClientGameState.cpp src/client/ClientGameState.h
        src/client/ServerConnector.cpp src/client/ServerConnector.h
        src/utils/messages/ClientMessage.cpp src/utils/messages/ClientMessage.h
        src/client/ServerHandler.cpp src/client/ServerHandler.h
        src/client/GuiHandler.cpp src/client/GuiHandler.h
        src/server/ClientConnector.cpp src/server/ClientConnector.h)

# Add executable server
set (SERVER_EXECUTABLE robots-server)
add_executable (${SERVER_EXECUTABLE}
        src/server/main.cpp
        src/server/ServerGameState.cpp src/server/ServerGameState.h
        src/server/ClientConnector.cpp src/server/ClientConnector.h
        src/server/ServerArguments.cpp src/server/ServerArguments.h
        src/server/ClientHandler.cpp src/server/ClientHandler.h
        src/server/ServerTcpAcceptor.cpp src/server/ServerTcpAcceptor.h
        src/utils/RandomNumberGenerator.cpp src/utils/RandomNumberGenerator.h
        src/utils/serializer.cpp src/utils/serializer.h
        src/utils/utils.cpp src/utils/utils.h
        src/utils/event.cpp src/utils/event.h
        src/utils/messages/ServerMessage.cpp src/utils/messages/ServerMessage.h
        src/server/ServerGameHandler.cpp src/server/ServerGameHandler.h
        src/utils/messages/ClientMessage.cpp src/utils/messages/ClientMessage.h
        src/utils/ConcurrentQueue.h
        src/utils/deserialization.cpp src/utils/deserialization.h
        src/client/ServerConnector.cpp src/client/ServerConnector.h
        src/utils/messages/InputMessage.cpp src/utils/messages/InputMessage.h)

# Setup BOOST library
find_package(Boost 1.71 COMPONENTS program_options system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()

target_link_libraries(${CLIENT_EXECUTABLE} LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${SERVER_EXECUTABLE} LINK_PUBLIC ${Boost_LIBRARIES})
