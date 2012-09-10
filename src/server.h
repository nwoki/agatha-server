/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include "config.h"
#include "commandexecuter.h"

#include <QtCore/QObject>

class Config;
class ServerAuthChecker;
class QUdpSocket;

/**
 * Agatha's UDP server. It listens for incoming messages from bots
 * @author Francesco Nwokeka
 */

class Server : public QObject
{
    Q_OBJECT

public:
    Server(Config::ServerConfigStruct serverCfg, CommandExecuter *cmdExe, QObject *parent = 0);
    ~Server();

private slots:
    void parseIncomingData();

private:
    CommandExecuter::Command determineCommand(const QString &command) const;

    Config::ServerConfigStruct m_serverConfigStruct;
    CommandExecuter *m_commandExecuter;
    ServerAuthChecker *m_serverAuthChecker;
    QUdpSocket *m_udpSocket;
};

#endif // SERVER_H
