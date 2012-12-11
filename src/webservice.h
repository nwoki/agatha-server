/*
 * webservice.h
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 * Author Francesco Nwokeka
 */

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QtNetwork/QTcpServer>

#include "config.h"

class WebService : public QTcpServer
{
    Q_OBJECT

public:
    WebService(Config::ServerConfigStruct configStruct, QObject *parent = 0);
    ~WebService();

private Q_SLOTS:
    void discardClient();       /** handles client socket delete when client socket gets disconnected */
    void onNewConnection();     /** called when a new connection is available */
    void readClient();          /** read data from client */

private:

    void setupConnects();       /** setup signal & slot connects for the web service */

    Config::ServerConfigStruct m_serverConfigStruct;
};

#endif // WEBSERVICE_H
