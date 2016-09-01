#project setup
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(card_reader CXX)

#set properties
set(DEBUG_OPTIONS -g)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  ${DEBUG_OPTIONS}")

#output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")

set(SRC_DIR "src")
set(INCLUDE_DIR "include")

include_directories(${INCLUDE_DIR})

#build library
file(GLOB_RECURSE LIB_SRC ${SRC_DIR}/lib/*.cpp)
add_library(lib ${LIB_SRC})


#add executable targets
add_executable(login.exe ${SRC_DIR}/login.cpp)
target_link_libraries(login.exe lib rt)

add_executable(read_db.exe ${SRC_DIR}/read_db.cpp)
target_link_libraries(read_db.exe lib)

add_executable(get_attendance.exe ${SRC_DIR}/get_attendance.cpp)
target_link_libraries(get_attendance.exe lib)

add_executable(export_to_csv.exe ${SRC_DIR}/export_db_to_csv.cpp)
target_link_libraries(export_to_csv.exe lib)
