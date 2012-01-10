/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "config.h"
#include "server.h"

#include <QtNetwork/QUdpSocket>

Server::Server(Config *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
    , m_udpSocket(new QUdpSocket(this))
{
    /// TODO bind port should be given by config
    m_udpSocket->bind(QHostAddress::Any, 1337);

    // connect on read signals
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(parseIncomingData()));
}

Server::~Server()
{
    delete m_udpSocket;
}

void Server::parseIncomingData()
{
    QByteArray rcvData;

    rcvData.resize(m_udpSocket->pendingDatagramSize());
    m_udpSocket->readDatagram(rcvData.data(), rcvData.size());

    /// TODO check token validity
    /// TODO make command executer class that runs received json
    qDebug() << "RCV: " << rcvData;
}

