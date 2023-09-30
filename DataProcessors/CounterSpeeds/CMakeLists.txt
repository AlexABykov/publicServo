cmake_minimum_required(VERSION 3.17)
project(CounterSpeeds)

set(CMAKE_CXX_STANDARD 14)

add_executable(CounterSpeeds
        main.cpp
        settings.h
        ServoModel/servoModel.cpp
        FilePrinter/filePrinter.cpp
        )