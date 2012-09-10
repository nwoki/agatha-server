project(agatha-server)

cmake_minimum_required(VERSION 2.6)

# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED)

set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTSQL TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# for debug
# -DDEBUG_MODE  (application debug mode)
# -g            (gdb symbols)
add_definitions(-g -DDEBUG_MODE)

include_directories(${QT_INCLUDE_DIR})

set(agatha-server_SRCS
    src/clierrorreporter.cpp
    src/config.cpp
    src/commandexecuter.cpp
    src/main.cpp
    src/responsedispatcher.cpp
    src/server.cpp
    src/checkers/geoipchecker.cpp
    src/checkers/serverAuthChecker.cpp
)

set(agatha-server_HDRS
    src/server.h
    src/config.h
    src/commandexecuter.h
    src/responsedispatcher.h
)

qt4_wrap_cpp(agatha-server_MOC_HDRS ${agatha-server_HDRS})

# qjson
add_subdirectory(src/qjson/src)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# final lib
add_executable(agatha-server ${agatha-server_SRCS} ${agatha-server_MOC_HDRS})


# link lib with Qt libs
target_link_libraries(agatha-server ${QT_LIBRARIES} qjson)

# tests
# enable_testing()
# add_subdirectory(tests)

# test app for sending custom text to srv
add_subdirectory(testapp)