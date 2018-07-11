cmake_minimum_required(VERSION 3.6)
project(ping)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
		incl/ping.h
		src/ping.c)

add_executable(ping ${SOURCE_FILES})