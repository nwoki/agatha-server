/*
 * webservice.h
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2014 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QtCore/QObject>

#include "config.h"

namespace RESTFul {
    class Server;
}

class WebService : public QObject
{
public:
    WebService(Config::CouchDbStruct couchDbStruct, quint16 port = 1337, QObject *parent = 0);
    ~WebService();

private:
    void setupConnects();                               /** setup signal & slot connects for the web service */

    Config::ServerConfigStruct m_serverConfigStruct;
    RESTFul::Server *m_server;
};

#endif // WEBSERVICE_H
