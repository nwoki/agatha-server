TARGET = agatha-server
TEMPLATE = app

QT -= gui
QT += core network sql

CONFIG += debug

MOC_DIR = ../.moc/
OBJECTS_DIR = ../.obj/
DESTDIR = ../

INCLUDEPATH += qjson/src DISTFILES

SOURCES += \
    clierrorreporter.cpp \
    commandexecuter.cpp \
    config.cpp \
    core.cpp \
    main.cpp \
    requesthandler.cpp \
    responsedispatcher.cpp \
    server.cpp \
    webservice.cpp \
    # CHECKERS
    checkers/geoipchecker.cpp \
    checkers/serverauthchecker.cpp \
    # QJSON PART
    qjson/src/parser.cpp \
    qjson/src/qobjecthelper.cpp \
    qjson/src/json_scanner.cpp \
    qjson/src/json_parser.cc \
    qjson/src/parserrunnable.cpp \
    qjson/src/serializer.cpp \
    qjson/src/serializerrunnable.cpp \



HEADERS += \
    clierrorreporter.h \
    commandexecuter.h \
    config.h \
    core.h \
    requesthandler.h \
    responsedispatcher.h \
    server.h \
    webservice.h \
    # CHECKERS
    checkers/geoipchecker.h \
    checkers/serverauthchecker.h \
    # QJSON PART
    qjson/src/parser.h \
    qjson/src/qobjecthelper.h \
    qjson/src/json_scanner.h \
    qjson/src/json_parser.hh \
    qjson/src/parserrunnable.h \
    qjson/src/serializer.h \
    qjson/src/serializerrunnable.h \
