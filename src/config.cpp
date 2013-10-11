/*
 * config.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"
#include "config.h"
#include "stdlib.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

Config::Config(QNetworkAccessManager *netManager, const QString &configFile, QObject* parent)
    : QSettings(configFile, QSettings::IniFormat, parent)
    , m_netManager(netManager)
{
    // be sure config file exists
    if (!QFile::exists(fileName())) {
        QString errorMsg("Can't find config file : '");
        errorMsg += fileName() +  "'";

        CliErrorReporter::printError(CliErrorReporter::APPLICATION
                                    , CliErrorReporter::CRITICAL
                                    , errorMsg);
        /// TODO define different error status?
        std::exit(1);
    }

    // populate the allowed databases
    m_allowedDbs.append("urbanterror_4_1_1");   // urban terror 4.1.1

    // load data
    loadConfigFile();

    // check if couch server is set up correctly and that we can communicate with it
    checkCouchDb();
}


Config::~Config()
{
    qDebug("[Config::~Config]");
}


void Config::checkCouchDb()
{
#ifdef DEBUG_MODE
    qDebug("[Config::checkCouchDb]");
#endif

    QByteArray reqUrl;
    QNetworkRequest req;

    CliErrorReporter::printNotification("[INFO] Checking Agatha server..");

    reqUrl += "http://" + m_couchDbStruct.ip + ":" + QString::number(m_couchDbStruct.port) + "/_all_dbs";

#ifdef DEBUG_MODE
    qDebug() << "[Config::checkCouchDb] request url: " << reqUrl;
#endif

    req.setUrl(QUrl(reqUrl));

    QNetworkReply *reply = m_netManager->get(req);

    connect(reply, SIGNAL(finished()), this, SLOT(onCheckCouchDbReceived()));
}


Config::CouchDbStruct Config::couchDbStruct() const
{
    return m_couchDbStruct;
}


void Config::onCheckCouchDbReceived()
{
    qDebug("[Config::onCheckCouchDbReceived]");

    // extract code
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "[Config::onCheckCouchDbReceived] Code is: " << httpCode;

    if (httpCode != 200) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE
        , CliErrorReporter::CRITICAL
        , "Can't communicate with the Agatha server @ " + m_couchDbStruct.ip);
        std::exit(1);
    }

    // check if db given already exists, otherwise create it

    /// TODO in the not so distant future, load more than one database from the config file.
    /// These will be the databases to handle for agatha. For now it's just UrbanTerror 4.1.1
    QList<QByteArray> dbs = reply->readAll().split('"');
    bool found = false;

    // don't need the reply any more
    reply->deleteLater();

    for (int i = 0; i < dbs.count() && !found; i++) {
        if (dbs.at(i) == m_couchDbStruct.dbName) {
            found = true;
        }
    }

    if (found) {
        CliErrorReporter::printNotification("[INFO] Agatha server found. Using " + m_couchDbStruct.dbName + " database");

        // notify that the configuration process has terminated
        Q_EMIT ready();
    } else {
        CliErrorReporter::printNotification("[INFO] Database not found. Creating '" + m_couchDbStruct.dbName + "' database");
        prepareCouchDb();
    }
}


void Config::onCreateDbReceived()
{
    qDebug("[Config::onCreateDbReceived]");

    // extract code
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "[Config::onCreateDbReceived] Code is: " << httpCode;

    // 201 -> created
    if (httpCode == 201) {
        CliErrorReporter::printNotification("[INFO] Database '" + m_couchDbStruct.dbName + "' created");

        // notify that the configuration process has terminated
        Q_EMIT ready();
    } else {
        CliErrorReporter::printError(CliErrorReporter::DATABASE
                                    , CliErrorReporter::ERROR
                                    , reply->errorString() + "\n" + reply->readAll());

        reply->deleteLater();
        std::exit(1);
    }

    reply->deleteLater();
}



void Config::onNetworkReplyReceived()
{
#ifdef DEBUG_MODE
    qDebug("[Config::onNetworkReplyReceived]");
#endif

    // extract code
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "Code is: " << httpCode;

//     if (httpCode == 404) {
//         return false;
//     }
}


bool Config::prepareCouchDb()
{
#ifdef DEBUG_MODE
    qDebug("[Config::prepareCouchDb]");
#endif

    // create the missing databas(es)
    QString createDbStr("http://");
    createDbStr += m_couchDbStruct.ip + ":" + QString::number(m_couchDbStruct.port) + "/" + m_couchDbStruct.dbName;

#ifdef DEBUG_MODE
    qDebug() << "[Config::prepareCouchDb] create db string: " << createDbStr;
#endif

    QNetworkRequest req;
    req.setUrl(QUrl(createDbStr));

    QNetworkReply *reply = m_netManager->put(req, createDbStr.toUtf8());

    connect(reply, SIGNAL(finished()), this, SLOT(onCreateDbReceived()));
}


Config::ServerConfigStruct Config::serverConfigStruct() const
{
    return m_serverConfigStruct;
}


void Config::loadConfigFile()
{
#ifdef DEBUG_MODE
    qDebug("[Config::loadConfigFile]");
#endif

    bool ok;
    CliErrorReporter::printNotification("[INFO] Loading configuration file..");

    // AGATHA SERVER
    beginGroup("server");
    int srvPort = value("port").toInt(&ok);

    if (!ok) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, "Invalid server port. Using default (1337)");
        srvPort = 1337;
    }

    m_serverConfigStruct.port = srvPort;
    endGroup();

    // COUCHDB
    beginGroup("agathaNode");

    m_couchDbStruct.ip = value("ip").toString();
    int dbPort = value("port").toInt(&ok);

    if (!ok) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, "Invalid couchDB port. Using default (5984)");
        dbPort = 5984;    // set to default couchdb port
    }

    m_couchDbStruct.port = dbPort;
    m_couchDbStruct.dbName = value("databaseName").toString();

    endGroup();

    qDebug() << "AGATHA SERVER PORT: " << m_serverConfigStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.ip;
    qDebug() << "COUCHDB : " << m_couchDbStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.dbName;

    // check if the db name is supported
    if (m_allowedDbs.contains(m_couchDbStruct.dbName)) {
        CliErrorReporter::printNotification("[INFO] Done!");
    } else {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION
                                    , CliErrorReporter::ERROR
                                    , "Database '" + m_couchDbStruct.dbName + "' not supported");
        std::exit(1);
    }
}

