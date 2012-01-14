/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"
#include "config.h"
#include "commandexecuter.h"
#include "geoipchecker.h"
#include "server.h"

#include "qjson/include/QJson/Parser"

#include <QtNetwork/QUdpSocket>

Server::Server(Config *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
    , m_commandExecuter(new CommandExecuter(new GeoIpChecker))
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
    delete m_udpSocket;
}

void Server::parseIncomingData()
{
    QByteArray rcvData;
    QHostAddress incomingIp;

    rcvData.resize(m_udpSocket->pendingDatagramSize());
    m_udpSocket->readDatagram(rcvData.data(), rcvData.size(), &incomingIp);

    qDebug() << "RCV: " << rcvData;

    QJson::Parser jsonParser;
    bool ok;

    QVariantMap result = jsonParser.parse(rcvData, &ok).toMap();

    if (!ok) {
        QString errStr("Recieved invalid json from ");
        errStr.append(incomingIp.toString());
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, errStr);
        return;
    }

#ifdef DEBUG_MODE
    qDebug() << result["token"].toString() << result["game"].toString() << result["command"].toString();
#endif

    /// TODO check token validity


    /// TODO if check passes, exec json

    // determine cmd and game to execute query for
    m_commandExecuter->execute(result["command"].toString(), result["game"].toString(), result["playerInfo"].toMap());
}

