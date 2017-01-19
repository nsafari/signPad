cmake_minimum_required(VERSION 3.6)
project(SerialCommunicationByC)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c protocol.h protocol.c)
add_executable(SerialCommunicationByC ${SOURCE_FILES})