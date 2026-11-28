cmake_minimum_required(VERSION 3.21)

project(dgc
    VERSION 2.1.0
    LANGUAGES C
    DESCRIPTION "A modern C23 project generator scaffold tool using CMake, Ninja, and vcpkg."
)

if(MSVC OR NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    message(FATAL_ERROR
        "${PROJECT_NAME} requires GCC or Clang because it uses C23 #embed and POSIX APIs.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME}_core STATIC
    src/templates.c
    src/command.c
    src/commands/new.c
    src/dgc/string.c
)

add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_core)

foreach(target IN ITEMS ${PROJECT_NAME}_core ${PROJECT_NAME})
    target_compile_features(${target} PRIVATE c_std_23)
    set_target_properties(${target} PROPERTIES C_EXTENSIONS OFF)

    target_include_directories(${target} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

    target_compile_definitions(${target} PRIVATE
        PROJECT_NAME="${PROJECT_NAME}"
        PROJECT_VERSION="${PROJECT_VERSION}"
        PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    )

    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Werror
    )
endforeach()

option(DGC_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

if(DGC_ENABLE_SANITIZERS)
    foreach(target IN ITEMS ${PROJECT_NAME}_core ${PROJECT_NAME})
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
    endforeach()

    target_link_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address,undefined
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
