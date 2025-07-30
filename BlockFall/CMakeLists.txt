cmake_minimum_required(VERSION 3.15)
project(BlockFall)

set(CMAKE_CXX_STANDARD 17)

add_executable(BlockFall source/main.cpp)

# -------------------------
# JSON setup (download if missing, otherwise use local)
# -------------------------
set(JSON_LOCAL_PATH "${CMAKE_SOURCE_DIR}/libs/JSON/include/nlohmann/json.hpp")

if (EXISTS ${JSON_LOCAL_PATH})
    message(STATUS "Using local nlohmann/json.hpp")
    target_include_directories(BlockFall PRIVATE ${CMAKE_SOURCE_DIR}/libs/JSON/include)
else()
    message(STATUS "Downloading nlohmann/json ...")
    include(FetchContent)

    # Create libs/JSON folder manually if not present
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/JSON)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_GetProperties(json)
    if (NOT json_POPULATED)
        FetchContent_Populate(json)
        # Copy extracted include folder to libs/JSON
        file(COPY ${json_SOURCE_DIR}/include DESTINATION ${CMAKE_SOURCE_DIR}/libs/JSON)
    endif()

    target_include_directories(BlockFall PRIVATE ${CMAKE_SOURCE_DIR}/libs/JSON/include)
endif()
# -------------------------
# SDL3 Setup (Cross-platform)
# -------------------------
if (WIN32)
    # Detect architecture
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "x64")
    else()
        set(ARCH_DIR "x86")
    endif()

    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3")
    include_directories(${SDL3_DIR}/include)

    set(SDL3_LIB_DIR "${SDL3_DIR}/lib/${ARCH_DIR}")
    set(SDL3_BIN_DIR "${SDL3_DIR}/bin/${ARCH_DIR}")

    target_link_directories(BlockFall PRIVATE ${SDL3_LIB_DIR})
    target_link_libraries(BlockFall PRIVATE SDL3)

    # Post-build copy DLL + config.json
    add_custom_command(TARGET BlockFall POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_BIN_DIR}/SDL3.dll"
            $<TARGET_FILE_DIR:BlockFall>
    )
    add_custom_command(TARGET BlockFall POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/config.json"
            $<TARGET_FILE_DIR:BlockFall>
    )

else() # Linux/macOS
    find_package(SDL3 REQUIRED CONFIG)
    target_link_libraries(BlockFall PRIVATE SDL3)

    # Install config.json next to the binary
    add_custom_command(TARGET BlockFall POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/config.json"
            $<TARGET_FILE_DIR:BlockFall>
    )
endif()
