/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "checkers/geoipchecker.h"
#include "clierrorreporter.h"
#include "commandexecuter.h"

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

CommandExecuter::CommandExecuter(Config::CouchDbStruct couchDbStruct, QObject *parent)
    : QObject(parent)
    , m_couchDbStruct(couchDbStruct)
    , m_geoIpChecker(new GeoIpChecker)
    , m_networkManager(new QNetworkAccessManager(this))
{
}


CommandExecuter::~CommandExecuter()
{
    delete m_geoIpChecker;
    delete m_networkManager;
}


void CommandExecuter::execute(Command cmd, const QString &token, const QString &game, const QVariantMap &player)
{
    ///TODO add geoloc position and timestamp before sending to couchDB. Cache data before writing to couch.

    qDebug() << "CommandExecuter::execute " << cmd << " for " << game;
    qDebug() << "Count " << player.count();

    qDebug() << player["nick"].toString();
    qDebug() << player["gear"].toString();
    qDebug() << player["ip"].toString();
    qDebug() << player["weaponMode"].toString();
    qDebug() << player["guid"].toString();

    /// TEST
    qDebug() << "PLAYER IP: " << player["ip"].toString() << " LOCATED @ " << m_geoIpChecker->location(player["ip"].toString());


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

        qDebug() << "REQUEST URL IS: " << requestUrl;

        m_reply = m_networkManager->get(request);

        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(onWhoIsReady()));
    }
}


void CommandExecuter::onReplyError(QNetworkReply::NetworkError error)
{
    CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::CRITICAL, m_reply->errorString());
    m_reply->deleteLater();
}


void CommandExecuter::onWhoIsReady()
{
    qDebug() << "[CommandExecuter::onWhoIsReady] : " << m_reply->readAll();

    /// TODO send back to bot
    m_reply->deleteLater();
}


