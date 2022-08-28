cmake_minimum_required(VERSION 3.23)
project(weight_machine C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(weight_machine
        src/macro.h
        src/om3.c
        src/REG_MG82FG5Bxx.h
        src/tm1640.c
        src/tm1640.h
        src/utility.c
        src/utility.h)
