cmake_minimum_required(VERSION 3.27)
project(KK_L3)

set(CMAKE_CXX_STANDARD 23)

add_executable(KK_L3_1 src/main1.cpp
        headers/AffineCoder.h
        headers/FilesUtil.h

        src/AffineCoder.cpp
)

add_executable(KK_L3_2 src/main2.cpp
        headers/AffineCoder.h
        headers/FilesUtil.h
        headers/BiGramMatrixGenerator.h

        src/AffineCoder.cpp
)
