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
    m_responseSocket->connectToHost(QHostAddress(recIp), recPort, QIODevice::WriteOnly);

    qDebug() << "SENDING PORT: " << recPort;
    /// TODO connects here
}


void ResponseDispatcher::sendResponse(const QByteArray& response)
{
    qDebug("[ResponseDispatcher::sendResponse]");
    if (!m_responseSocket) {
        /// TODO handle error
    }

    m_responseSocket->write(response);
}



