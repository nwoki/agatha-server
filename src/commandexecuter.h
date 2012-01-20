/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef COMMANDEXECUTER_H
#define COMMANDEXECUTER_H

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
    enum RequestType {
        GET,
        POST,
        PUT
    };

    CommandExecuter(QObject *parent = 0);
    ~CommandExecuter();

    /**
     * execute given command for the game specified using info in the player VariantMap
     * @param command query type to execute on database
     * @param game gametype to execute query for
     * @param player QVariantMap with info on the player to use in query
     */
    void execute(const QString &command, const QString &game, const QVariantMap &player);

    void test();

private slots:
    void onReadyRead();
    void onReplyError(QNetworkReply::NetworkError error);

private:
    void sendRequest(const QNetworkRequest &request, RequestType type);

    GeoIpChecker *m_geoIpChecker;
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
};



#endif  // COMMANDEXECUTER_H
