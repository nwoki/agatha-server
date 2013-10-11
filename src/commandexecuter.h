/*
 * commandexecuter.h
 *
 * This file is part of agathaServer
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef COMMANDEXECUTER_H
#define COMMANDEXECUTER_H

#include "config.h"

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

/**
 * Class used for executing incoming commands from various bots around the world!
 * This class interacts with Agatha's couchDB database
 * @author Francesco Nwokeka
 */

class GeoIpChecker;
class QNetworkAccessManager;
class QTcpSocket;
// class ResponseDispatcher;

class CommandExecuter : public QObject
{
    Q_OBJECT

public:
    enum Command {
        ADD,
        BAN,
        IS_BANNED,
        INVALID_COMMAND,
        WHO_IS
    };

    CommandExecuter(Config::CouchDbStruct couchDbStruct, QNetworkAccessManager *netManager, QObject *parent = 0);
    ~CommandExecuter();

    /**
     * execute given command for the game specified using info in the player VariantMap
     * @param command query type to execute on database
     * @param token server token
     * @param game gametype to execute query for
     * @param player QVariantMap with info on the player to use in query
     * @param httpSocket socket of the connection with the bot to respond to
     */
    void execute(Command cmd
                , const QString &token
                , const QString &game
                , const QVariantMap &player
                , QTcpSocket *httpSocket);

private slots:
    /** Generic error slot for QNetworkReply */
    void onReplyError(QNetworkReply::NetworkError error);

    /** slot called after a WHOIS request */
    void onWhoIsFinished();

private:
    Config::CouchDbStruct m_couchDbStruct;
//     GeoIpChecker *m_geoIpChecker;    // TODO
    QNetworkAccessManager *m_networkManager;

    QHash<QString, QTcpSocket*> m_httpSocketHash;    // a hash of the sockets ordered by gameserver hash
};



#endif  // COMMANDEXECUTER_H
