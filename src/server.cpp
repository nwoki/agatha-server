/*
 * server.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "checkers/serverAuthChecker.h"
#include "clierrorreporter.h"
#include "server.h"

#include "qjson/include/QJson/Parser"

#include <QtNetwork/QUdpSocket>

Server::Server(Config::ServerConfigStruct serverCfg, CommandExecuter *cmdExe, QObject *parent)
    : QObject(parent)
    , m_serverConfigStruct(serverCfg)
    , m_commandExecuter(cmdExe)
    , m_serverAuthChecker(new ServerAuthChecker)
    , m_udpSocket(new QUdpSocket(this))
{
    m_udpSocket->bind(QHostAddress::Any, m_serverConfigStruct.port);

    // connect on read signals
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(parseIncomingData()));
}


Server::~Server()
{
    delete m_commandExecuter;
    delete m_serverAuthChecker;
    delete m_udpSocket;
}


CommandExecuter::Command Server::determineCommand(const QString &command) const
{
    CommandExecuter::Command cmd;

    if (command.toLower() == "add") {
        cmd = CommandExecuter::ADD;
    } else if (command.toLower() == "ban") {
        cmd = CommandExecuter::BAN;
    } else if (command.toLower() == "isbanned") {
        cmd = CommandExecuter::IS_BANNED;
    } else if (command.toLower() == "whois") {
        cmd = CommandExecuter::WHO_IS;
    }

#ifdef DEBUG_MODE
    qDebug() << "[Server::determineCommand] command is: " << command.toLower();
#endif

    return cmd;
}


void Server::parseIncomingData()
{
    QByteArray rcvData;
    QHostAddress incomingIp;
    quint16 incomingPort;

    rcvData.resize(m_udpSocket->pendingDatagramSize());
    m_udpSocket->readDatagram(rcvData.data(), rcvData.size(), &incomingIp, &incomingPort);

#ifdef DEBUG_MODE
    qDebug() << "RCV DATA: " << rcvData;
#endif

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

    QString srvToken = result["token"].toString();
    QString game = result["game"].toString();
    QString command = result["command"].toString();

#ifdef DEBUG_MODE
    qDebug() << srvToken << game << command << incomingIp.toString() << incomingPort;
#endif

    // check token validity
    if (m_serverAuthChecker->isTokenValid(incomingIp.toString(), incomingPort, srvToken)) {
        m_commandExecuter->execute(determineCommand(command)
                                , srvToken
                                , game
                                , result["playerInfo"].toMap()
                                , incomingIp.toString()
                                , incomingPort);
    } else {
        // else drop incoming json
        qDebug("INVALID TOKEN");
    }
}

