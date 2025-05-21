#
# CMakeLists.txt
# Copyright (c) 2025 Vinodh Kumar Markapuram <GreenHex@gmail.com>
#
# 13-Apr-2025
# 

cmake_minimum_required(VERSION 3.10)
project(linked_list
		VERSION 1.0
		DESCRIPTION "Linked List Naive Implementation"
    	LANGUAGES C)

set(EXE_NAME ${PROJECT_NAME}-{PROJECT_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

add_executable(${PROJECT_NAME}
	./linked_list.c
	./example.c
)

# add_compile_definitions(NDEBUG)
# add_compile_definitions(DEBUG)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
