/*
 * webservice.cpp
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 * Author Francesco Nwokeka
 */


#include "commandexecuter.h"
#include "requesthandler.h"
#include "webservice.h"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>

#include <QtNetwork/QTcpSocket>

// WebService::WebService(Config::ServerConfigStruct configStruct, QObject *parent)
WebService::WebService(Config::CouchDbStruct couchDbStruct, quint16 port, QObject *parent)
    : QTcpServer(parent)
    , m_requestHandler(new RequestHandler(this))
    , m_commandExecuter(new CommandExecuter(couchDbStruct, this))
{
    qDebug("[WebService::WebService]");

    // set config info
    m_serverConfigStruct.port = port;

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


void WebService::parseIncomingMessage(QTcpSocket *socket)
{
    qDebug("[WebService::parseIncomingMessage]");

    // extrack info, might come in handy
    QString message = socket->readAll();
    QString peerAddr = socket->peerAddress().toString();
    quint16 peerPort = socket->peerPort();

    qDebug() << "[WebService::parseIncomingMessage] Peer: " << peerAddr;
    qDebug() << "[WebService::parseIncomingMessage] Port: " << peerPort;
    qDebug() << "[WebService::parseIncomingMessage] Message" << message;

    // extract request type
    QStringList responseParts = message.split(QRegExp("[ \r\n][ \r\n]*"));

    qDebug() << "[WebService::parseIncomingMessage] responseParts: " << responseParts;

    // TODO check token + ip validity (discuss this with seven)

//     GET: ("GET", "/", "HTTP/1.1", "User-Agent:", "curl/7.28.1", "Host:", "127.0.0.1:1337", "Accept:", "*/*", "")
    // last item in the list is the json data
    if (responseParts.first() == "GET") {
        m_requestHandler->handleGetRequest(responseParts.last().toUtf8());
    } else if (responseParts.first() == "POST") {
        m_requestHandler->handlePostRequest(responseParts.last().toUtf8());
    } else if (responseParts.first() == "PUT") {
        m_requestHandler->handlePutRequest(responseParts.last().toUtf8());
    }
}


void WebService::readClient()
{
    qDebug("[WebService::readClient]");

    QTcpSocket *socket  = qobject_cast<QTcpSocket*>(sender());

    // should never happen!
    if (!socket) {
        return;
    }

    parseIncomingMessage(socket);

    // close once i've finished reading
    socket->close();
}


void WebService::setupConnects()
{
    qDebug("[WebService::setupConnects]");

    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}