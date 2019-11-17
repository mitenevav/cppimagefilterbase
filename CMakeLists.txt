cmake_minimum_required(VERSION 2.8)

project(student_toolkit)
add_executable(${PROJECT_NAME} "main.cpp" "png_toolkit.cpp" "png_toolkit.h"
    "stb_image.h" "stb_image_write.h" "Kernel.cpp" "Kernel.h" "Filters.cpp" "Filters.h" "ImageData.h" "Manager.cpp" "Manager.h")
