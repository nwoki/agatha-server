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

class QJsonObject;

/**
 * Class that handles the RESTFUL requests parsed from the webservice
 */

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    RequestHandler(QObject *parent = 0);
    ~RequestHandler();

    void handleGetRequest(const QByteArray &json);
    void handlePostRequest(const QByteArray &json);
    void handlePutRequest(const QByteArray &json);

private:
    CommandExecuter::Command command(const QJsonObject &jsonObj);  // returns command value taken from json info
    QJsonObject jsonObject(const QByteArray &jsonData);            // transforms the json data to a QJsonObject
};

#endif // REQUESTHANDLER_H
