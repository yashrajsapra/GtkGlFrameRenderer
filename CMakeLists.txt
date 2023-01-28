cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

project(GTKGL)

find_package(OpenCV REQUIRED)
find_package (glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(AFTER SYSTEM include)
include_directories(${GTK3_INCLUDE_DIRS})
SET(CORE_FILES
    include/matrix.h
    include/util.h
    include/model.h
    include/program.h
    include/background.h
    include/gui.hpp
    include/view.h

    src/matrix.cpp
    src/model.cpp
    src/program.cpp
    src/background.cpp
    src/gui.cpp
    src/view.cpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SOURCE
	${CORE_FILES}
)

add_library(gtkglapp STATIC ${SOURCE})

add_executable(main main.cpp )

target_include_directories(gtkglapp PRIVATE
    ${GLFW_INCLUDE_DIRS}
)

target_include_directories(main PUBLIC
    ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(main PUBLIC
    gtkglapp
    GL
    glfw 
    GLEW::GLEW
    ${GTK3_LIBRARIES}
)