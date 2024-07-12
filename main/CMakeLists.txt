# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Add all source files in the current directory and the include directory
file(GLOB SOURCES "*.c" "include/*.c")

# Create an executable named "app" from the source files
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS "include")
