cmake_minimum_required(VERSION 3.19)
project(Web)

set(CMAKE_CXX_STANDARD 11)

add_executable(Web main.cpp Request.cpp Cgi.cpp Response.cpp Sockets.cpp Config.cpp Server.cpp Location.cpp Utils.cpp)
