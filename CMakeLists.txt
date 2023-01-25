cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(GLSL_COMPILER /usr/bin/glslangValidator)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GDK_PIXBUF REQUIRED gdk-pixbuf-2.0)
pkg_check_modules(GLIB REQUIRED glib-2.0 gio-2.0)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenCV REQUIRED)
find_package (glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED) 

find_library(GTK_LIB libgtk-3.so PATHS /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH)
find_library(GLIB libglib-2.0.so PATHS /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH)

include_directories(AFTER SYSTEM include)

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

target_include_directories(gtkglapp PUBLIC
    ${OPENGL_INCLUDE_DIRS}  
    ${GLUT_INCLUDE_DIRS} 
    /usr/include/gtk-3.0
    /usr/include/glib-2.0
    /usr/include/pango-1.0
    /usr/include/cairo
    /usr/include/gdk-pixbuf-2.0
    /usr/include/gdk-pixbuf-2.0
    /usr/include/atk-1.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
)
# file(GLOB_RECURSE SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/cube/*.glsl ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bkgd/*.glsl)
set(SHADER_DIR "shaders")
file(GLOB_RECURSE GLSL_FILES "shaders/bkgd/*.glsl")
foreach(file ${GLSL_FILES})
    get_filename_component(file_name ${file} NAME)
    message ("FILES=====>" ${file} )
    set(bin_file ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.o)
    add_custom_command(OUTPUT ${bin_file}
                       COMMAND objcopy -I binary -O elf64-x86-64 -B i386 ${file} ${bin_file}
                       DEPENDS ${file}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMENT "Converting ${file} to binary")
    list(APPEND BIN_FILES ${bin_file})
endforeach()
add_custom_target(binary_shaders DEPENDS ${BIN_FILES})

# file(GLOB_RECURSE GLSL_FILES1 "shaders/cube/*.glsl")
# foreach(file ${GLSL_FILES1})
#     get_filename_component(file_name ${file} NAME)
#     message ("CUBE FILES=====>" ${file} )
#     set(bin_file1 ${CMAKE_CURRENT_BINARY_DIR}/cube/${file_name}.bin)
#     add_custom_command(OUTPUT ${bin_file1}
#                        COMMAND objcopy -I binary -O elf64-x86-64 -B i386 ${file} ${bin_file1}
#                        DEPENDS ${file}
#                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                        COMMENT "Converting ${file} to binary")
#     list(APPEND BIN_FILES ${bin_file1})
# endforeach()
# add_custom_target(binary_shaders1 DEPENDS ${BIN_FILES1})

add_executable(main main.cpp ) #${SHADER_FILES}
add_dependencies(main binary_shaders ) #binary_shaders1
target_include_directories(main PUBLIC
    ${OPENGL_INCLUDE_DIRS}  
    ${GLUT_INCLUDE_DIRS} 
    /usr/include/gtk-3.0
    /usr/include/at-spi-2.0
    /usr/include/at-spi2-atk/2.0
    /usr/include/gio-unix-2.0/
	/usr/include/glib-2.0
    /usr/include/pango-1.0
    /usr/include/cairo
	/usr/include/gdk-pixbuf-2.0
    /usr/include/atk-1.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include

    /usr/include/dbus-1.0
    /usr/lib/x86_64-linux-gnu/dbus-1.0/include 
    /usr/include/gtk-3.0 
    /usr/include/gio-unix-2.0/ 
    /usr/include/cairo 
    /usr/include/pango-1.0 
    /usr/include/harfbuzz 
    /usr/include/pango-1.0 
    /usr/include/atk-1.0 
    /usr/include/cairo 
    /usr/include/pixman-1 
    /usr/include/freetype2 
    /usr/include/libpng16 
    /usr/include/freetype2 
    /usr/include/libpng16 
    /usr/include/gdk-pixbuf-2.0 
    /usr/include/libpng16 
    /usr/include/glib-2.0 
    /usr/lib/x86_64-linux-gnu/glib-2.0/include 
    /usr/include/libdrm
)
message("GTK LIB" ${GTK_LIB} ${OPENGL_LIBRARIES})
target_link_libraries(main PUBLIC
    # binary_shaders
    ${OPENGL_LIBRARIES}
    ${GLIB}
    ${GLFW3_LIBRARY}
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARY}
    ${OpenCV_LIBS}
    ${GTK_LIB}
	${GLIB_LIBRARIES}
	# ${GLIB_LDFLAGS}
    ${GLEW_LIBRARIES}
    ${GDK_PIXBUF_LIBRARIES}
    glfw 
    GLEW::GLEW
    GL
    # glsl_compile
    /usr/lib/x86_64-linux-gnu/libGLEW.so
    # /usr/lib/x86_64-linux-gnu/libgdk_pixbuf-2.0.so
    /usr/lib/x86_64-linux-gnu/libglib-2.0.a
	/usr/lib/x86_64-linux-gnu/libgdk-3.so
	/usr/lib/x86_64-linux-gnu/libcairo.so
	/usr/lib/x86_64-linux-gnu/libgobject-2.0.so
	/usr/lib/x86_64-linux-gnu/libglib-2.0.so
	/usr/lib/x86_64-linux-gnu/libpcre.so
    ${OPENGL_LIBRARIES}

    # /usr/lib/x86_64-linux-gnu/libgdk_pixbuf-2.0.so
    ${CMAKE_THREAD_LIBS_INIT}
    gtkglapp
    -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lGL
    )

