/*
 * commandexecuter.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "checkers/geoipchecker.h"
#include "clierrorreporter.h"
#include "commandexecuter.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

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
    qDebug("[CommandExecuter::execute]");

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

        /*
         * add the gameserver token (couchdb _id given to the gameserver automatically) so that i can identify the request
         * when it comes back
         */
        reply->setProperty("token", token);

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(finished()), this, SLOT(onWhoIsFinished()));
    }

    /*
     * store the http socket with the corrisponding token (unique). This way i still have the tcp socket with the bot that sent
     * the request and can send back the answer
     */
    m_httpSocketHash.insert(token, httpSocket);
}


void CommandExecuter::onReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[CommandExecuter::onReplyError]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::CRITICAL, reply->errorString());
    m_httpSocketHash.take(reply->property("token").toString())->close();
    reply->deleteLater();
}


void CommandExecuter::onWhoIsFinished()
{
    qDebug("[CommandExecuter::onWhoIsFinished]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString replyToken = reply->property("token").toString();
    QByteArray response = reply->readAll();

    // delete what we don't need anymore
    reply->deleteLater();

    qDebug() << "[CommandExecuter::onWhoIsFinished] : " << response << " from reply with name: " << replyToken;

    // get socket used to communicate with the bot
    QTcpSocket *httpSocket = m_httpSocketHash.take(replyToken);

    // safety check
    if (httpSocket == Q_NULLPTR) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::ERROR, "[CommandExecuter::onWhoIsFinished] response socket is null!");
        return;
    }

    // send couch db info back to the bot
    httpSocket->write(response);

    // close the socket, we don't need it anymore
    httpSocket->close();
}
