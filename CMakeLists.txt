cmake_minimum_required(VERSION 3.20)
project(dgc 
    VERSION 2.0.1
    LANGUAGES C
    DESCRIPTION "A modern C23 project generator scaffold tool using CMake, Ninja, and vcpkg."
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    message(FATAL_ERROR
        "dgc requires GCC or Clang because it uses C23 #embed and POSIX APIs.")
endif()

add_executable(${PROJECT_NAME} 
    src/main.c
    src/templates.c
    src/command.c
    src/commands/new.c
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
else()
    message(FATAL_ERROR
        "Unsupported C compiler: ${CMAKE_C_COMPILER_ID}. Use GCC or Clang.")
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)