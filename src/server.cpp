/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "server.h"

#include <QtNetwork/QUdpSocket>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_udpSocket(new QUdpSocket(this))
{
    m_udpSocket->bind(QHostAddress::LocalHost, 1337);

    // connect on read signals
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
}

Server::~Server()
{
    delete m_udpSocket;
}

void Server::readIncomingData()
{
    QByteArray rcvData;

    while(m_udpSocket->hasPendingDatagrams()) {
        // read all
        rcvData.append(m_udpSocket->readLine());
    }
}

