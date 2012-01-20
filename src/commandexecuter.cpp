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

CommandExecuter::CommandExecuter(QObject *parent)
    : QObject(parent)
    , m_geoIpChecker(new GeoIpChecker)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

CommandExecuter::~CommandExecuter()
{
    delete m_geoIpChecker;
    delete m_networkManager;
}

void CommandExecuter::execute(const QString &command, const QString &game, const QVariantMap &player)
{
    ///TODO add geoloc position and timestamp before sending to couchDB

    qDebug() << "CommandExecuter::execute " << command << " for " << game;
    qDebug() << "Count " << player.count();

    qDebug() << player["nick"].toString();
    qDebug() << player["gear"].toString();
    qDebug() << player["ip"].toString();
    qDebug() << player["weaponMode"].toString();
    qDebug() << player["guid"].toString();

    /// TEST
    qDebug() << "PLAYER IP: " << player["ip"].toString() << " LOCATED @ " << m_geoIpChecker->location(player["ip"].toString());
}

void CommandExecuter::test()
{
    /// TODO modify with config values
    QNetworkRequest request(QUrl("http://127.0.0.1:5984"));
    sendRequest(request, GET);
}

void CommandExecuter::onReplyError(QNetworkReply::NetworkError error)
{
    CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::CRITICAL, m_reply->errorString());
    m_reply->deleteLater();
}

void CommandExecuter::onReadyRead()
{
    qDebug() << "[CommandExecuter::onReplyFinished] " << m_reply->readAll();
    m_reply->deleteLater();
}

void CommandExecuter::sendRequest(const QNetworkRequest &request, RequestType type)
{
    if (type == GET) {
        m_reply = m_networkManager->get(request);
    }
    /// TODO POST
    /* else {
        reply = m_networkManager->post(request, "ASDASDASD");
    }*/

    // connect reply to slots
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}




