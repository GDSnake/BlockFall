cmake_minimum_required(VERSION 3.15)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

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
    ${CMAKE_SOURCE_DIR}/source
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

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
# SDL3 Setup (download if missing)
# ---------------------------------------------------------
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3")

if (EXISTS "${SDL3_DIR}/include/SDL3/SDL.h")
    message(STATUS "Using local SDL3 at ${SDL3_DIR}")
else()
    message(STATUS "Downloading SDL3 precompiled binaries ...")
    file(MAKE_DIRECTORY ${SDL3_DIR})

    if (WIN32)
        set(SDL3_URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.18/SDL3-devel-3.2.18-VC.zip)
    else()
        set(SDL3_URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.18/SDL3-3.2.18.tar.gz)
    endif()

    set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)
    FetchContent_Declare(
        SDL3
        URL ${SDL3_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_Populate(SDL3)

    message(STATUS "SDL3 downloaded to: ${sdl3_SOURCE_DIR}")
    file(GLOB CONTENTS "${sdl3_SOURCE_DIR}/*")
    message(STATUS "Contents: ${CONTENTS}")

    set(SDL3_COPY_DIR "")
    if (EXISTS "${sdl3_SOURCE_DIR}/include/SDL3/SDL.h")
        set(SDL3_COPY_DIR "${sdl3_SOURCE_DIR}")
    else()
        foreach(child ${CONTENTS})
            if (EXISTS "${child}/include/SDL3/SDL.h")
                set(SDL3_COPY_DIR "${child}")
                break()
            endif()
        endforeach()
    endif()

    if (SDL3_COPY_DIR STREQUAL "")
        message(FATAL_ERROR "SDL3 extraction failed: include/SDL3/SDL.h not found.")
    else()
        message(STATUS "Found SDL3 extracted in: ${SDL3_COPY_DIR}")
    endif()

    file(COPY "${SDL3_COPY_DIR}/include" DESTINATION "${SDL3_DIR}")
    if (EXISTS "${SDL3_COPY_DIR}/lib")
        file(COPY "${SDL3_COPY_DIR}/lib" DESTINATION "${SDL3_DIR}")
    endif()
    if (EXISTS "${SDL3_COPY_DIR}/bin")
        file(COPY "${SDL3_COPY_DIR}/bin" DESTINATION "${SDL3_DIR}")
    endif()
endif()

# ---------------------------------------------------------
# SDL3_image Setup (download if missing)
# ---------------------------------------------------------

if (WIN32)
    set(SDL3_IMAGE_URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-devel-3.2.4-VC.zip)
else()
    set(SDL3_IMAGE_URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz)
endif()

set(SDL3_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3_image")

if (EXISTS "${SDL3_IMAGE_DIR}/include/SDL3/SDL_image.h")
    message(STATUS "Using local SDL3_image at ${SDL3_IMAGE_DIR}")
else()
    message(STATUS "Downloading SDL3_image precompiled binaries ...")

    FetchContent_Declare(
        SDL3_image
        URL ${SDL3_IMAGE_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_Populate(SDL3_image)

    # Copy includes and libs into project libs folder
    file(MAKE_DIRECTORY ${SDL3_IMAGE_DIR})
    file(COPY "${sdl3_image_SOURCE_DIR}/include" DESTINATION "${SDL3_IMAGE_DIR}")
    if (EXISTS "${sdl3_image_SOURCE_DIR}/lib")
        file(COPY "${sdl3_image_SOURCE_DIR}/lib" DESTINATION "${SDL3_IMAGE_DIR}")
    endif()
    if (EXISTS "${sdl3_image_SOURCE_DIR}/bin")
        file(COPY "${sdl3_image_SOURCE_DIR}/bin" DESTINATION "${SDL3_IMAGE_DIR}")
    endif()
endif()

# Include & link SDL3_image
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_IMAGE_DIR}/include)

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL3_IMAGE_LIB_DIR "${SDL3_IMAGE_DIR}/lib/x64")
    else()
        set(SDL3_IMAGE_LIB_DIR "${SDL3_IMAGE_DIR}/lib/x86")
    endif()

    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL3_IMAGE_LIB_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_IMAGE_LIB_DIR}/SDL3_image.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image)
endif()

# ---------------------------------------------------------
# SDL3_mixer Setup (download if missing)
# ---------------------------------------------------------
 set(SDL3_MIXER_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3")

 if (EXISTS "${SDL3_MIXER_DIR}/include/SDL3/SDL_mixer.h")
     message(STATUS "Using local SDL3_mixer at ${SDL2_MIXER_DIR}")
 endif()
# else()
#     message(STATUS "Downloading SDL2_mixer precompiled binaries ...")
#     file(MAKE_DIRECTORY ${SDL2_MIXER_DIR})


#     if (WIN32)
#         set(SDL2_MIXER_URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-VC.zip)
#     else()
#         set(SDL2_MIXER_URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-2.8.1.tar.gz)
#     endif()

#     FetchContent_Declare(
#         SDL2_mixer
#         URL ${SDL2_MIXER_URL}
#         DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#     )
    
#     set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

#     FetchContent_Declare(
#         SDL2_mixer
#         URL ${SDL2_MIXER_URL}
#         DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#     )
#     FetchContent_MakeAvailable(SDL2_mixer)

#     message(STATUS "SDL2_mixer downloaded to: ${sdl2_mixer_SOURCE_DIR}")

#     # Recursively find include/SDL2/SDL_mixer.h
#     file(GLOB_RECURSE MIXER_HEADER_PATH "${sdl2_mixer_SOURCE_DIR}/include/SDL_mixer.h")

#     if (NOT MIXER_HEADER_PATH)
#         message(FATAL_ERROR "SDL2_mixer extraction failed: include/SDL3/SDL_mixer.h not found.")
#     endif()

#     # Get directory that contains include
#     get_filename_component(MIXER_INCLUDE_DIR "${MIXER_HEADER_PATH}" DIRECTORY)
#     while(NOT "${MIXER_INCLUDE_DIR}" STREQUAL "${sdl2_mixer_SOURCE_DIR}")
#         if (EXISTS "${MIXER_INCLUDE_DIR}/include/SDL3/SDL_mixer.h")
#             break()
#         endif()
#         get_filename_component(MIXER_INCLUDE_DIR "${MIXER_INCLUDE_DIR}" DIRECTORY)
#     endwhile()

#     message(STATUS "Found SDL2_mixer extracted in: ${MIXER_INCLUDE_DIR}")

# file(COPY "${MIXER_INCLUDE_DIR}/include/SDL_mixer.h" DESTINATION "${SDL3_DIR}/include/SDL3")    if (EXISTS "${MIXER_INCLUDE_DIR}/lib")
#         file(COPY "${MIXER_INCLUDE_DIR}/lib" DESTINATION "${SDL2_MIXER_DIR}")
#     endif()
#     if (EXISTS "${MIXER_INCLUDE_DIR}/bin")
#         file(COPY "${MIXER_INCLUDE_DIR}/bin" DESTINATION "${SDL2_MIXER_DIR}")
#     endif()
# endif()

# ---------------------------------------------------------
# SDL3 Linking
# ---------------------------------------------------------
if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "x64")
    else()
        set(ARCH_DIR "x86")
    endif()

    include_directories(${SDL3_DIR}/include)

    set(SDL3_LIB_DIR "${SDL3_DIR}/lib/${ARCH_DIR}")

    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL3_LIB_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SDL_DLL "SDL3d.dll")
    else()
        set(SDL_DLL "SDL3.dll")
    endif()

    if (NOT EXISTS "${SDL3_LIB_DIR}/${SDL_DLL}")
        set(SDL_DLL "SDL3.dll")
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_LIB_DIR}/${SDL_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    find_package(SDL3 REQUIRED CONFIG)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif()

set(SDL3_MIXER_LIB_DIR "${SDL3_DIR}/lib/${ARCH_DIR}")


target_include_directories(BlockFall PRIVATE ${SDL3_MIXER_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_mixer)

add_custom_command(TARGET BlockFall POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_MIXER_LIB_DIR}/SDL3_mixer.dll"
        $<TARGET_FILE_DIR:BlockFall>
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