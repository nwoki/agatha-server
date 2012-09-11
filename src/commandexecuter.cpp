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
#include "responsedispatcher.h"

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

CommandExecuter::CommandExecuter(Config::CouchDbStruct couchDbStruct, QObject *parent)
    : QObject(parent)
    , m_couchDbStruct(couchDbStruct)
    , m_geoIpChecker(new GeoIpChecker)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_responseDispatcher(new ResponseDispatcher(this))
{
    /// TODO make srv->couchDB methods synchronous
    /*
     * Q AsyncClass *class;                                                                                                                            *
     * QEventLoop e;
     *
     * connect(class, SIGNAL(finished()), &e, SLOT(quit()));
     *
     * class->performAsyncOp();
     * e.exec();
     *
     * // exec returns when performAsyncOp has ended. Class is stuck in a loop but
     * the application isn't blocked, as e is a children of the main event loop
     */
}


CommandExecuter::~CommandExecuter()
{
    delete m_geoIpChecker;
    delete m_networkManager;
}


void CommandExecuter::execute(Command cmd
                            , const QString &token
                            , const QString &game
                            , const QVariantMap &player
                            , const QString &responseIp
                            , quint16 responsePort)
{
    ///TODO add geoloc position and timestamp before sending to couchDB. Cache data before writing to couch.

    qDebug() << "CommandExecuter::execute " << cmd << " for " << game;
    qDebug() << "Count " << player.count();
    qDebug() << "Send response to: " << responseIp << " : " << responsePort;

    qDebug() << player["nick"].toString();
    qDebug() << player["gear"].toString();
    qDebug() << player["ip"].toString();
    qDebug() << player["weaponMode"].toString();
    qDebug() << player["guid"].toString();

#ifdef DEBUG_MODE
    qDebug() << "[CommandExecuter::execute] PLAYER IP: " << player["ip"].toString() << " LOCATED @ " << m_geoIpChecker->location(player["ip"].toString());
#endif

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
        m_responseDispatcher->setResponseRecipient(responseIp, responsePort);

        requestUrl.append(player["guid"].toString());
        requestUrl.append("-");
        requestUrl.append(token);
        request.setUrl(requestUrl);

#ifdef DEBUG_MODE
        qDebug() << "[CommandExecuter::execute] REQUEST URL IS: " << requestUrl;
#endif

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
    QByteArray response = m_reply->readAll();
#ifdef DEBUG_MODE
    qDebug() << "[CommandExecuter::onWhoIsReady] : " << response;
#endif

    /// TODO send back to bot
    /// TODO filter response message to extract only the desired player object. Don't
    /// need all the extra stuff couchDB sends back
    m_responseDispatcher->sendResponse(response);

    m_reply->deleteLater();
}


void CommandExecuter::clearResponseData()
{
//     m_responseIp.clear();
}


void CommandExecuter::setResponseData(const QString &responseIp, quint16 responsePort)
{
    m_responseDispatcher->setResponseRecipient(responseIp, responsePort);
}




