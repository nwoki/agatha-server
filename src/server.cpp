/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "checkers/serverAuthChecker.h"
#include "clierrorreporter.h"
#include "config.h"
#include "commandexecuter.h"
#include "server.h"

#include "qjson/include/QJson/Parser"

#include <QtNetwork/QUdpSocket>

Server::Server(Config *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
    , m_commandExecuter(new CommandExecuter)
    , m_serverAuthChecker(new ServerAuthChecker)
    , m_udpSocket(new QUdpSocket(this))
{
    m_udpSocket->bind(QHostAddress::Any, m_config->serverConfigStruct().port);

    // connect on read signals
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(parseIncomingData()));
}

Server::~Server()
{
    delete m_commandExecuter;
    delete m_config;
    delete m_serverAuthChecker;
    delete m_udpSocket;
}

void Server::parseIncomingData()
{
    QByteArray rcvData;
    QHostAddress incomingIp;
    quint16 incomingPort;

    rcvData.resize(m_udpSocket->pendingDatagramSize());
    m_udpSocket->readDatagram(rcvData.data(), rcvData.size(), &incomingIp, &incomingPort);

    qDebug() << "RCV: " << rcvData;

    QJson::Parser jsonParser;
    bool ok;

    QVariantMap result = jsonParser.parse(rcvData, &ok).toMap();

    if (!ok) {
        QString errStr("Recieved invalid json from ");
        errStr.append(incomingIp.toString());
        errStr.append(":");
        errStr.append(incomingPort);
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, errStr);
        return;
    }

#ifdef DEBUG_MODE
    qDebug() << result["token"].toString() << result["game"].toString() << result["command"].toString();
#endif

    // check token validity
    if (m_serverAuthChecker->isTokenValid(incomingIp.toString(), incomingPort, result["token"].toString())) {
        // determine cmd and game to execute query for
        m_commandExecuter->execute(result["command"].toString(), result["game"].toString(), result["playerInfo"].toMap());
    } else {
        // else drop incoming json
        qDebug("INVALID TOKEN");
    }
}

