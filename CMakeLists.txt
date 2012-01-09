project(srvAgatha)

cmake_minimum_required(VERSION 2.6)

# Find Qt4. Need this to link and create static lib
FIND_PACKAGE(Qt4 REQUIRED)

set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# for debug
add_definitions(-DDB_DEBUG -DDEBUG_MODE)

include_directories(${QT_INCLUDE_DIR})

set(srvAgatha_SRCS
    src/main.cpp
    src/server.cpp
)

set(srvAgatha_HDRS
    src/server.h
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