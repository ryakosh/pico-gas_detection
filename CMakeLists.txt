cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(gas_leakage_detector
        gas_leakage_detector.c
        )

target_link_libraries(gas_leakage_detector pico_stdlib)
pico_add_extra_outputs(gas_leakage_detector)
