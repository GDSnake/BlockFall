cmake_minimum_required(VERSION 3.15)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(BlockFall)

# Macro to run SDL setup scripts
macro(run_sdl3_setup external_dir)
    # Only run setup if directory is empty
    file(GLOB CHILDREN "${external_dir}/*")
    list(LENGTH CHILDREN child_count)
    if(child_count EQUAL 0)
        if(WIN32)
            set(SCRIPT "${external_dir}/Get-GitModules.ps1")
            if(EXISTS ${SCRIPT})
                message(STATUS "Running ${SCRIPT} ...")
                execute_process(
                    COMMAND powershell -ExecutionPolicy Bypass -File "${SCRIPT}"
                    WORKING_DIRECTORY ${external_dir}
                    RESULT_VARIABLE result
                )
                if(NOT result EQUAL 0)
                    message(FATAL_ERROR "Failed to run ${SCRIPT}")
                endif()
            endif()
        else()
            set(SCRIPT "${external_dir}/download.sh")
            if(EXISTS ${SCRIPT})
                message(STATUS "Running ${SCRIPT} ...")
                execute_process(
                    COMMAND /bin/sh "${SCRIPT}"
                    WORKING_DIRECTORY ${external_dir}
                    RESULT_VARIABLE result
                )
                if(NOT result EQUAL 0)
                    message(FATAL_ERROR "Failed to run ${SCRIPT}")
                endif()
            endif()
        endif()
    else()
        message(STATUS "Skipping SDL setup for ${external_dir}, already populated")
    endif()
endmacro()


set(CMAKE_CXX_STANDARD 23)

# Force CMake to build DLLs (shared libs)
#set(BUILD_SHARED_LIBS ON)

# Enable parallel compilation with MSVC
if (MSVC)
    add_compile_options(/MP)
endif()

file(GLOB_RECURSE PROJECT_SOURCES source/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS source/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Define globally for all targets
add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)
# --- Windows specific subsystem settings ---
if (WIN32)
    # Console for Debug
    set_target_properties(BlockFall PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
    )

    # Windowed app for Release
    set_target_properties(BlockFall PROPERTIES
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/source
)

if (WIN32)
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/resources/app.rc
    )
endif()
# ---------------------------------------------------------
# Create a default config.json if missing
# ---------------------------------------------------------
set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/config.json")
if (NOT EXISTS ${CONFIG_FILE})
    file(WRITE ${CONFIG_FILE} "{\n")
    file(APPEND ${CONFIG_FILE} "    \"title\": \"Default Game Name\",\n")
    file(APPEND ${CONFIG_FILE} "    \"window_width\": 1280,\n")
    file(APPEND ${CONFIG_FILE} "    \"window_height\": 720,\n")
    file(APPEND ${CONFIG_FILE} "    \"fullscreen\": false\n")
    file(APPEND ${CONFIG_FILE} "}\n")
    message(STATUS "Created default config.json")
endif()

include(FetchContent)

# ---------------------------------------------------------
# JSON Setup (download if missing, otherwise use local)
# ---------------------------------------------------------
set(JSON_LOCAL_PATH "${CMAKE_SOURCE_DIR}/libs/JSON/include/nlohmann/json.hpp")

if (EXISTS ${JSON_LOCAL_PATH})
    message(STATUS "Using local nlohmann/json.hpp")
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/JSON/include)
else()
    message(STATUS "Downloading nlohmann/json ...")
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/JSON)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(json)
    file(COPY ${json_SOURCE_DIR}/include DESTINATION ${CMAKE_SOURCE_DIR}/libs/JSON)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/JSON/include)
endif()

# ---------------------------------------------------------
# Ensure NASM is available for SDL_mixer assembly code
# ---------------------------------------------------------
if(WIN32)
    find_program(NASM_EXECUTABLE nasm)

    if(NOT NASM_EXECUTABLE)
        message(STATUS "NASM not found, downloading prebuilt NASM...")

        include(FetchContent)
        set(NASM_URL "https://www.nasm.us/pub/nasm/releasebuilds/2.16.02/win64/nasm-2.16.02-win64.zip")
        set(NASM_ZIP "${CMAKE_BINARY_DIR}/nasm.zip")
        set(NASM_DIR "${CMAKE_BINARY_DIR}/nasm")

        file(DOWNLOAD ${NASM_URL} ${NASM_ZIP} SHOW_PROGRESS)
        file(MAKE_DIRECTORY ${NASM_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${NASM_ZIP} WORKING_DIRECTORY ${NASM_DIR})

        # NASM executable path inside extracted folder
        set(NASM_EXECUTABLE "${NASM_DIR}/nasm-2.16.02/nasm.exe")
        if(EXISTS ${NASM_EXECUTABLE})
            message(STATUS "NASM downloaded to ${NASM_EXECUTABLE}")
        else()
            message(FATAL_ERROR "Failed to download NASM")
        endif()
    endif()

    # Tell CMake to use this NASM
    set(CMAKE_ASM_NASM_COMPILER "${NASM_EXECUTABLE}" CACHE FILEPATH "NASM assembler")
endif()

# SDL3 stack (via submodules)
set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE)

# Run setup scripts for SDL_image and SDL_mixer
run_sdl3_setup("${CMAKE_SOURCE_DIR}/3rdParty/SDL_image/external")
run_sdl3_setup("${CMAKE_SOURCE_DIR}/3rdParty/SDL_mixer/external")

add_subdirectory(3rdParty/SDL)
add_subdirectory(3rdParty/SDL_image)
add_subdirectory(3rdParty/SDL_mixer)
add_subdirectory(3rdParty/SDL_ttf)

target_link_libraries(${PROJECT_NAME} PRIVATE
     SDL3::SDL3
     SDL3_image::SDL3_image
     SDL3_mixer::SDL3_mixer
     SDL3_ttf::SDL3_ttf
 )

# ---------------------------------------------------------
# Copy config.json after build
# ---------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.json"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# ---------------------------------------------------------
# Copy assets folder after build
# ---------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Copy DLLs after build (Windows only)
# ---------------------------------------------------------
if (WIN32)
    foreach(DLL SDL3 SDL3_image SDL3_mixer SDL3_ttf)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${DLL}::${DLL}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
endif()