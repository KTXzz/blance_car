cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME blance_car)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        User/BSP/OLED/OLED.c
        User/BSP/OLED/OLED.h
        User/BSP/Motor/Motor.c
        User/BSP/Motor/Motor.h
        test/motor_test.c
        test/motor_test.h
        User/BSP/MPU6050/mpu6050.c
        User/BSP/MPU6050/mpu6050.h
        User/APP/APP_oled/APP_oled.c
        User/APP/APP_oled/APP_oled.h
        User/APP/control/control.c
        User/APP/control/control.h
        User/BSP/Encoder/encoder.c
        User/BSP/Encoder/encoder.h
        User/APP/APP_PID/pid.c
        User/APP/APP_PID/pid.h
        User/APP/balance/balance.c
        User/APP/balance/balance.h
        User/BSP/BLUETOOTH/bluetooth.c
        User/BSP/BLUETOOTH/bluetooth.h
        User/APP/APP_Bluetooth/APP_bluetooth.c
        User/APP/APP_Bluetooth/APP_bluetooth.h
        User/APP/speed/speed.c
        User/APP/speed/speed.h
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # User/BSP/OLED/OLED.c
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        User/BSP/OLED
        User/BSP/Motor
        User/BSP/MPU6050
        User/APP/APP_oled
        User/APP/control
        User/BSP/Encoder
        User/APP/APP_PID
        User/APP/balance
        User/BSP/BLUETOOTH
        User/APP/APP_Bluetooth
        User/APP/speed
        test
        # Add user defined include path
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -u _printf_float)