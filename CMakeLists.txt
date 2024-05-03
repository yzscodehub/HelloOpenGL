cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# STB 宏
#add_definitions(-DSTB_IMAGE_IMPLEMENTATION)


set(THIRD_PARTY_DIR "3rdParty")

project(HelloOpenGL)

option(HELLO_VULKAN_BUILD_THIRDPARTY "Build thirdparty" ON)
if(HELLO_VULKAN_BUILD_THIRDPARTY)
    add_subdirectory(3rdParty)
endif()

include_directories(${THIRD_PARTY_DIR})
include_directories(${THIRD_PARTY_DIR}/glm)

link_directories()

set(SOURCE_DIR "src")
file(GLOB_RECURSE CPP_SOURCES ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.c)
add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    glfw
    assimp)


