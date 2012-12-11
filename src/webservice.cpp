/*
 * webservice.cpp
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 * Author Francesco Nwokeka
 */

#include "webservice.h"

#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>

// WebService::WebService(quint16 port, QObject *parent)
WebService::WebService(Config::ServerConfigStruct configStruct, QObject *parent)
    : QTcpServer(parent)
    , m_serverConfigStruct(configStruct)
{
    qDebug("[WebService::WebService]");

    // TODO check port binding validity?
//     if (m_port < 1024) {
//         
//     }

    // TODO handle binding error
    if (listen(QHostAddress::Any, m_serverConfigStruct.port)) {
        qDebug() << "[WebService::WebService] listening on: " << m_serverConfigStruct.port;
        setupConnects();
    } else {
        qDebug() << "[WebService::WebService] not listening on: " << m_serverConfigStruct.port;
    }
}

WebService::~WebService()
{
}


void WebService::discardClient()
{
    qDebug("[WebService::discardClient]");

    QTcpSocket *socket = (QTcpSocket*)sender();
    socket->deleteLater();

    qDebug("[WebService::discardClient] client disconnected");
}


void WebService::onNewConnection()
{
    qDebug("[WebService::onNewConnection]");

    if (!hasPendingConnections()) {
        return;
    }

    QTcpSocket *socket = nextPendingConnection();

    // should never happen
    if (!socket) {
        return;
    }

    // connect the socket for deletion later
    connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(discardClient()));
}


void WebService::readClient()
{
    qDebug("[WebService::readClient]");

    QTcpSocket *socket  = qobject_cast<QTcpSocket*>(sender());

    // should never happen!
    if (!socket) {
        return;
    }

    // TODO parse message
    qDebug() << socket->readAll();

    // close once i've finished reading
    socket->close();
}


void WebService::setupConnects()
{
    qDebug("[WebService::setupConnects]");

    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}