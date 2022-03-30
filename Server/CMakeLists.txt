cmake_minimum_required(VERSION 3.21)
project(Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server main.cpp main.h UDPSocket.cpp UDPSocket.h Request.h RequestData.h enums.h utils.h utils.cpp Server.cpp Server.h ResponseData.h Response.h)
