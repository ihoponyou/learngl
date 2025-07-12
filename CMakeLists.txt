cmake_minimum_required(VERSION 3.20)
project(learngl CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#####################################################################
#                       EXTERNAL DEPENDENCIES
#####################################################################

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
    target_compile_options(${libName} PRIVATE "-w")
endfunction()

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_git_dependency(
    glfw
    https://github.com/glfw/glfw.git
    3.4
)

add_git_dependency(
    glad
    https://github.com/Dav1dde/glad.git
    v0.1.36
)

add_git_dependency(
    glm
    https://github.com/g-truc/glm
    1.0.1
)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_WARNINGS_AS_ERRORS OFF)
option(BUILD_SHARED_LIBS OFF)
add_git_dependency(
    assimp
    https://github.com/assimp/assimp
    v6.0.2
)

find_package(OpenGL REQUIRED)

#####################################################################
#                        EXECUTABLE SETUP
#####################################################################

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE} "${CMAKE_CURRENT_LIST_DIR}/lib/")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm assimp OpenGL::GL)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

#####################################################################
#                        RESOURCE SETUP
#####################################################################
file(COPY "${CMAKE_CURRENT_LIST_DIR}/src/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

#####################################################################
#                        PACKAGE SETUP
#####################################################################

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
include(CPack)
