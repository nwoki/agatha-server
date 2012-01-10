/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>

class QUdpSocket;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
    ~Server();

private slots:
    void parseIncomingData();

private:
    QUdpSocket *m_udpSocket;
};

#endif // SERVER_H
