cmake_minimum_required (VERSION 3.5.1)
project (main)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER "g++")

find_package(OpenCV HINTS "/usr/include/opencv4" )
find_package(OpenMP)

file(GLOB SOURCES "src/*.cpp" "src/epitome/*.cpp")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(inc)

add_executable(epitome_fft.exe ${SOURCES})

target_link_libraries(epitome_fft.exe ${OpenCV_LIBS})
target_link_libraries(epitome_fft.exe fftw3)
target_link_libraries(epitome_fft.exe config++)
target_link_libraries(epitome_fft.exe boost_filesystem)
target_link_libraries(epitome_fft.exe OpenMP::OpenMP_CXX)