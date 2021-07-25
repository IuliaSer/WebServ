cmake_minimum_required(VERSION 3.19)
project(ourServ2)

set(CMAKE_CXX_STANDARD 11)

add_executable(ourServ2 main.cpp Request.cpp Cgi.cpp Response.cpp Sockets.cpp Config.cpp Server.cpp Location.cpp Utils.cpp)