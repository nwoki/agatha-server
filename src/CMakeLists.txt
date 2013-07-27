set(agatha-server_SRCS
    checkers/geoipchecker.cpp
    checkers/serverauthchecker.cpp
    clierrorreporter.cpp
    config.cpp
    commandexecuter.cpp
    core.cpp
    main.cpp
    requesthandler.cpp
    webservice.cpp
)

set(agatha-server_HDRS
    core.h
    config.h
    commandexecuter.h
    requesthandler.h
    webservice.h
)

qt5_wrap_cpp(agatha-server_MOC_HDRS ${agatha-server_HDRS})

# final lib
add_executable(agatha-server ${agatha-server_SRCS})

qt5_use_modules(agatha-server Core Network Sql)

# link lib with Qt libs
target_link_libraries(agatha-server qjson)
