cmake_minimum_required(VERSION 3.0)
project(printer-controller VERSION 1.0)

add_executable(printer-controller 
    src/main.c
    src/config.c
)
target_include_directories(printer-controller PUBLIC include)

target_link_libraries(printer-controller PUBLIC wiringPi)
