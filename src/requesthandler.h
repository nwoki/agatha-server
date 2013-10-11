/*
 * requesthandler.cpp
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "commandexecuter.h"

#include <QtCore/QObject>

class CommandExecuter;
class QJsonObject;
class QTcpSocket;

/**
 * Class that handles the RESTFUL requests parsed from the webservice
 */

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    RequestHandler(Config::CouchDbStruct couchDbStruct, QNetworkAccessManager *netManager, QObject *parent = 0);
    ~RequestHandler();

    void handleHttpRequest(const QByteArray &json, QTcpSocket *httpSocket);

private:
    CommandExecuter::Command command(const QJsonObject &jsonObj);  // returns command value taken from json info
    QJsonObject jsonObject(const QByteArray &jsonData);            // transforms the json data to a QJsonObject

    CommandExecuter *m_commandExecuter;
};

#endif // REQUESTHANDLER_H
