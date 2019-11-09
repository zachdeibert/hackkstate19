cmake_minimum_required(VERSION 3.0)
project(printer-controller VERSION 1.0)

add_executable(printer-controller 
    src/main.c
)

target_link_libraries(printer-controller PUBLIC wiringPi)
