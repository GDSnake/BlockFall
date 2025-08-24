cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# MSVC runtime: static link for release/debug
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

project(BlockFall)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE PROJECT_SOURCES
    source/*.cpp
    source/AssetManagers/*.cpp
    source/Config/*.cpp
    source/Input/*.cpp
    source/Entities/*.cpp
    source/Game/*.cpp
)

file(GLOB_RECURSE PROJECT_HEADERS
    source/*.h
    source/AssetManagers/*.h
    source/Config/*.h
    source/Input/*.h
    source/Entities/*.h
    source/Game/*.h
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Define globally for all targets
add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/source/AssetManagers
    ${CMAKE_SOURCE_DIR}/source/Config
    ${CMAKE_SOURCE_DIR}/source/Core
    ${CMAKE_SOURCE_DIR}/source/Input
    ${CMAKE_SOURCE_DIR}/source/Entities
    ${CMAKE_SOURCE_DIR}/source/Game
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

if (WIN32)
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
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

# ------------------------------
# Windows: download and extract zips
# ------------------------------
if (WIN32)
    include(ExternalProject)

    set(SDL3_URL       "https://github.com/libsdl-org/SDL/releases/download/release-3.2.18/SDL3-devel-3.2.18-VC.zip")
    set(SDL3_IMAGE_URL "https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-devel-3.2.4-VC.zip")
    #set(SDL3_MIXER_URL "https://github.com/libsdl-org/SDL_mixer/releases/download/release-3.2.4/SDL3_mixer-devel-3.2.4-VC.zip")

    set(LIBS_DIR "${CMAKE_BINARY_DIR}/libs")
    file(MAKE_DIRECTORY ${LIBS_DIR})

    # Helper to download & extract
    function(download_and_extract name url)
        set(outdir "${LIBS_DIR}/${name}")
        if(NOT EXISTS ${outdir})
            message(STATUS "Downloading ${name}...")
            file(DOWNLOAD ${url} ${LIBS_DIR}/${name}.zip SHOW_PROGRESS)
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBS_DIR}/${name}.zip
                    WORKING_DIRECTORY ${LIBS_DIR})
        endif()
    endfunction()

    download_and_extract("SDL3"       ${SDL3_URL})
    download_and_extract("SDL3_image" ${SDL3_IMAGE_URL})
    #download_and_extract("SDL3_mixer" ${SDL3_MIXER_URL})

    # Paths inside the extracted zips
    set(SDL3_DIR       "${LIBS_DIR}/SDL3")
    set(SDL3_IMAGE_DIR "${LIBS_DIR}/SDL3_image")
    #set(SDL3_MIXER_DIR "${LIBS_DIR}/SDL3_mixer")

    target_include_directories(${PROJECT_NAME} PRIVATE
            ${SDL3_DIR}/include
            ${SDL3_IMAGE_DIR}/include
            ${SDL3_MIXER_DIR}/include
    )

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            ${SDL3_DIR}/lib/x64/SDL3.lib
            ${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.lib
            ${SDL3_MIXER_DIR}/lib/x64/SDL3_mixer.lib
    )

    # Copy DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL3_DIR}/lib/x64/SDL3.dll
            ${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll
            ${SDL3_MIXER_DIR}/lib/x64/SDL3_mixer.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    # ------------------------------
    # macOS: Fetch from GitHub
    # ------------------------------
elseif(APPLE)
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.20
    )
    FetchContent_MakeAvailable(SDL3)

    FetchContent_Declare(
            SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-3.2.4
    )
    FetchContent_MakeAvailable(SDL3_image)

    FetchContent_Declare(
            SDL3_mixer
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
            GIT_TAG release-3.2.4
    )
    #FetchContent_MakeAvailable(SDL3_mixer)

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            SDL3::SDL3
            SDL3_image::SDL3_image
            SDL3_mixer::SDL3_mixer
    )

    # ------------------------------
    # Linux: same as macOS
    # ------------------------------
else()
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.20
    )
    FetchContent_MakeAvailable(SDL3)

    FetchContent_Declare(
            SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-3.2.4
    )
    FetchContent_MakeAvailable(SDL3_image)

    FetchContent_Declare(
            SDL3_mixer
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
            GIT_TAG release-3.2.4
    )
    #FetchContent_MakeAvailable(SDL3_mixer)

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            SDL3::SDL3
            SDL3_image::SDL3_image
            SDL3_mixer::SDL3_mixer
    )
endif()


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