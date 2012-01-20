/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef COMMANDEXECUTER_H
#define COMMANDEXECUTER_H

#include "config.h"

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

class CommandExecuter : public QObject
{
    Q_OBJECT

public:
    enum Command {
        ADD,
        BAN,
        IS_BANNED,
        WHO_IS
    };

    CommandExecuter(Config::CouchDbStruct couchDbStruct, QObject *parent = 0);
    ~CommandExecuter();

    /**
     * execute given command for the game specified using info in the player VariantMap
     * @param command query type to execute on database
     * @param token server token
     * @param game gametype to execute query for
     * @param player QVariantMap with info on the player to use in query
     */
    void execute(Command cmd, const QString &token, const QString &game, const QVariantMap &player);

private slots:
    void onReplyError(QNetworkReply::NetworkError error);
    void onWhoIsReady();    /** slot called after a WHOIS request */

private:
//     void checkForExistingRecord();

    Config::CouchDbStruct m_couchDbStruct;
    GeoIpChecker *m_geoIpChecker;
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
};



#endif  // COMMANDEXECUTER_H
