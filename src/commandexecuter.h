/*
 * commandexecuter.h
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
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
class ResponseDispatcher;

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

    CommandExecuter(Config::CouchDbStruct couchDbStruct, QObject *parent = 0);
    ~CommandExecuter();

    /**
     * execute given command for the game specified using info in the player VariantMap
     * @param command query type to execute on database
     * @param token server token
     * @param game gametype to execute query for
     * @param player QVariantMap with info on the player to use in query
     * @param responseIp ip to return response to if the command requests an answer
     * @param responePort port to return response to if the command requests an answer
     */
    void execute(Command cmd
                , const QString &token
                , const QString &game
                , const QVariantMap &player
                , const QString &responseIp
                , quint16 responsePort);

private slots:
    /** Generic error slot for QNetworkReply */
    void onReplyError(QNetworkReply::NetworkError error);

    /** slot called after a WHOIS request */
    void onWhoIsReady();

private:
//     void checkForExistingRecord();
    void clearResponseData();

    /**
     * Set data for response. This data is accessed from those functions which require to send a response
     * to the bot that sent the following requests: WHO_IS, IS_BANNED
     */
    void setResponseData(const QString &responseIp, quint16 responsePort);

    Config::CouchDbStruct m_couchDbStruct;
//     GeoIpChecker *m_geoIpChecker;    // TODO
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
    ResponseDispatcher *m_responseDispatcher;
};



#endif  // COMMANDEXECUTER_H
