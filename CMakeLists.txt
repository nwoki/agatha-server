project(srvAgatha)

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
add_definitions(-g)

include_directories(${QT_INCLUDE_DIR})

set(srvAgatha_SRCS
    src/clierrorreporter.cpp
    src/config.cpp
    src/commandexecuter.cpp
    src/geoipchecker.cpp
    src/main.cpp
    src/server.cpp
)

set(srvAgatha_HDRS
    src/server.h
    src/config.h
)

qt4_wrap_cpp(srvAgatha_MOC_HDRS ${srvAgatha_HDRS})

# qjson
add_subdirectory(src/qjson/src)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# final lib
add_executable(srvAgatha ${srvAgatha_SRCS} ${srvAgatha_MOC_HDRS})


# link lib with Qt libs
target_link_libraries(srvAgatha ${QT_LIBRARIES} qjson)

# tests
# enable_testing()
# add_subdirectory(tests)

# test app for sending custom text to srv
add_subdirectory(testapp)