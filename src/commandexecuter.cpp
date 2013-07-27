/*
 * commandexecuter.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "checkers/geoipchecker.h"
#include "clierrorreporter.h"
#include "commandexecuter.h"

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>

CommandExecuter::CommandExecuter(Config::CouchDbStruct couchDbStruct, QObject *parent)
    : QObject(parent)
    , m_couchDbStruct(couchDbStruct)
//     , m_geoIpChecker(new GeoIpChecker)
    , m_networkManager(new QNetworkAccessManager(this))
{
}


CommandExecuter::~CommandExecuter()
{
//     delete m_geoIpChecker;
}


void CommandExecuter::execute(Command cmd
                            , const QString &token
                            , const QString &game
                            , const QVariantMap &player
                            , QTcpSocket *httpSocket)
{
    ///TODO add geoloc position and timestamp before sending to couchDB. Cache data before writing to couch.
    // TODO format queries according to what's written on the wiki

    qDebug() << "CommandExecuter::execute " << cmd << " for " << game;
    qDebug() << "Count " << player.count();

    qDebug() << player["nick"].toString();
    qDebug() << player["gear"].toString();
    qDebug() << player["ip"].toString();
    qDebug() << player["weaponMode"].toString();
    qDebug() << player["guid"].toString();

    QNetworkRequest request;
    QString requestUrl(m_couchDbStruct.queryUrl());

    if (cmd == ADD) {
        /// TODO
        /*
        * 1 - check if player doesn't already exist in db. If he doesn't go to point 4
        * 2 - get id and _rev
        * 3 - add data in the right places
        * 4 - push to database
        */
    } else if (cmd == BAN) {

    } else if (cmd == IS_BANNED) {

    } else if (cmd == WHO_IS) {
        requestUrl.append(player["guid"].toString());
        requestUrl.append("-");
        requestUrl.append(token);
        request.setUrl(requestUrl);

        qDebug() << "[CommandExecuter::execute] REQUEST URL IS: " << requestUrl;

        QNetworkReply *reply = m_networkManager->get(request);
        reply->setProperty("token", token);

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(readyRead()), this, SLOT(onWhoIsReady()));
    }

    // store the http socket with the corrisponding token (unique)
    m_httpSocketHash.insert(token, httpSocket);
}


void CommandExecuter::onReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[CommandExecuter::onReplyError]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::CRITICAL, reply->errorString());
    m_httpSocketHash.value(reply->property("token").toString())->close();
    reply->deleteLater();
}


void CommandExecuter::onWhoIsReady()
{
    qDebug("[CommandExecuter::onWhoIsReady]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString replyToken = reply->property("token").toString();
    QByteArray response = reply->readAll();

    qDebug() << "[CommandExecuter::onWhoIsReady] : " << response << " from reply with name: " << reply->property("token").toString();

    // send couch db info back to the bot
    m_httpSocketHash.value(reply->property("token").toString())->write(response);

    // close the socket, we don't need it anymore
    m_httpSocketHash.value(reply->property("token").toString())->close();

    reply->deleteLater();
}
