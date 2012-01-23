/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef RESPONSEDISPATCHER_H
#define RESPONSEDISPATCHER_H

#include <QtCore/QObject>

class QUdpSocket;

class ResponseDispatcher : public QObject
{
    Q_OBJECT

public:
    ResponseDispatcher(QObject *parent = 0);
    ~ResponseDispatcher();

    /**
     * Send response to previously ser response recipient
     * @param response packet to send
     */
    void sendResponse(const QByteArray &response);

    /**
     * Set recipient for next reponse packet
     * @param recIp recipient ip address
     * @param recPort recipient port
     */
    void setResponseRecipient(const QString &recIp, quint16 recPort);

private:
    QString m_recipientIp;
    quint16 m_recipientPort;

    QUdpSocket *m_responseSocket;
};



#endif  // RESPONSEDISPATCHER_H