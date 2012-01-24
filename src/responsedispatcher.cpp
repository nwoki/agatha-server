/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "responsedispatcher.h"

#include <QtNetwork/QUdpSocket>

ResponseDispatcher::ResponseDispatcher(QObject* parent)
    : QObject(parent)
    , m_responseSocket(new QUdpSocket)
{
}


ResponseDispatcher::~ResponseDispatcher()
{
    m_responseSocket->deleteLater();
}


void ResponseDispatcher::setResponseRecipient(const QString& recIp, quint16 recPort)
{
    qDebug() << "[ResponseDispatcher::setResponseRecipient]: recipientIp: " << recIp << " recPort: " << recPort;

    m_responseSocket->connectToHost(QHostAddress(recIp), recPort, QIODevice::WriteOnly);

    qDebug() << "SENDING ip:port: " << recIp << ":" << recPort;
    /// TODO connects here
}


void ResponseDispatcher::sendResponse(const QByteArray& response)
{
    qDebug() << "[ResponseDispatcher::sendResponse]: " << response << " to " << m_recipientIp << " : " << m_recipientPort;

    if (!m_responseSocket) {
        /// TODO handle error (should never happen)
    }

    m_responseSocket->write(response);

    // always disconnect from host to enable new connection
    m_responseSocket->disconnectFromHost();
}



