/*
 * webservice.h
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QtNetwork/QTcpServer>

#include "config.h"

class QNetworkAccessManager;
class RequestHandler;


/* NOTE
 * Use POST to create a new resource and PUT to update a known resource
 */

class WebService : public QTcpServer
{
    Q_OBJECT

public:
    WebService(Config::CouchDbStruct couchDbStruct, QNetworkAccessManager *netManager, quint16 port = 1337, QObject *parent = 0);
    ~WebService();

private Q_SLOTS:
    void discardClient();                               /** handles client socket delete when client socket gets disconnected */
    void onNewConnection();                             /** called when a new connection is available */
    void readClient();                                  /** read data from client */

private:

    void parseIncomingMessage(QTcpSocket *socket);      /** parses incoming GET POST PUT messages */
    void setupConnects();                               /** setup signal & slot connects for the web service */

    Config::ServerConfigStruct m_serverConfigStruct;
    RequestHandler *m_requestHandler;
};

#endif // WEBSERVICE_H
