cmake_minimum_required(VERSION 3.27)
project(KK_L3)

set(CMAKE_CXX_STANDARD 23)

add_executable(KK_L3 src/main.cpp
        headers/AffineCoder.h
        headers/FilesUtil.h

        src/AffineCoder.cpp
)
