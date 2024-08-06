cmake_minimum_required(VERSION 3.28)
project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

# Ensure LLVM_DIR is set correctly
set(LLVM_DIR "/path/to/llvm")  # Update this to your LLVM installation path

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Define LLVM_DEFINITIONS and include directories
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# Handle RTTI
if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Source files
set(SOURCES
        src/Main.cpp
        src/AST.cpp
        src/Codegen.cpp
        src/Lexer.cpp
        src/DebugInfo.cpp
        src/Parser.cpp
        src/Main.cpp
)

# Executable
add_executable(toy ${SOURCES}
        src/Main.cpp)

# Link against the required LLVM libraries
llvm_map_components_to_libnames(LLVM_LIBS core orcjit native)
target_link_libraries(toy ${LLVM_LIBS})

# Special linker flags for Darwin (macOS)
if(APPLE)
    target_link_libraries(toy "-undefined dynamic_lookup")
endif()

# Output CMake and LLVM settings for debugging
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake CXX flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "LLVM_DIR: ${LLVM_DIR}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries: ${LLVM_LIBS}")
