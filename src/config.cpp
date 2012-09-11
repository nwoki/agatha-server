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

Config::Config(const QString &configFile, QObject* parent)
    : QSettings(configFile, QSettings::IniFormat, parent)
    , m_netManager(new QNetworkAccessManager(this))
    , m_networkReply(0)
{
    // be sure config file exists
    if (!QFile::exists(fileName())) {
        QString errorMsg("Can't find config file : '");
        errorMsg.append(fileName());
        errorMsg.append("'");

        CliErrorReporter::printError(CliErrorReporter::APPLICATION
                                    , CliErrorReporter::CRITICAL
                                    , errorMsg);
        /// TODO define different error status?
        std::exit(1);
    }

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
    m_networkReply = m_netManager->get(req);

    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onCheckCouchDbReceived()));
}


Config::CouchDbStruct Config::couchDbStruct() const
{
    return m_couchDbStruct;
}


void Config::onCheckCouchDbReceived()
{
#ifdef DEBUG_MODE
    qDebug("[Config::onCheckCouchDbReceived]");
#endif

    // extract code
    int httpCode = m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "Code is: " << httpCode;

    if (httpCode != 200) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE
        , CliErrorReporter::CRITICAL
        , "Can't communicate with the Agatha server @ " + m_couchDbStruct.ip);
        std::exit(1);
    }

    // check if db given already exists, otherwise create it

    /// TODO in the not so distant future, load more than one database from the config file.
    /// These will be the databases to handle for agatha. For now it's just UrbanTerror 4.1.1
    QList<QByteArray> dbs = m_networkReply->readAll().split('"');
    bool found = false;

    // don't need the reply any more
    m_networkReply->deleteLater();

    for (int i = 0; i < dbs.count() && !found; i++) {
        if (dbs.at(i) == m_couchDbStruct.dbName) {
            found = true;
        }
    }

    if (found) {
        CliErrorReporter::printNotification("[INFO] Agatha server found. Using " + m_couchDbStruct.dbName + " database");
    } else {
        CliErrorReporter::printNotification("[INFO] Database not found. Creating '" + m_couchDbStruct.dbName + "' database");
        prepareCouchDb();
    }
}


void Config::onCreateDbReceived()
{
#ifdef DEBUG_MODE
    qDebug("[Config::onCreateDbReceived]");
#endif

    // extract code
    int httpCode = m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "Code is: " << httpCode;

    // 201 -> created
    if (httpCode == 201) {
        CliErrorReporter::printNotification("[INFO] Database '" + m_couchDbStruct.dbName + "' created");
    } else {
        CliErrorReporter::printError(CliErrorReporter::DATABASE
                                    , CliErrorReporter::ERROR
                                    , m_networkReply->errorString() + "\n" + m_networkReply->readAll());
        std::exit(1);
    }

    m_networkReply->deleteLater();
}



void Config::onNetworkReplyReceived()
{
#ifdef DEBUG_MODE
    qDebug("[Config::onNetworkReplyReceived]");
#endif

    // extract code
    int httpCode = m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

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

    m_networkReply = m_netManager->put(req, createDbStr.toUtf8());

    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onCreateDbReceived()));
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
    beginGroup("couchDb");

    m_couchDbStruct.ip = value("ip").toString();
    int dbPort = value("port").toInt(&ok);

    if (!ok) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, "Invalid couchDB port. Using default (5984)");
        dbPort = 5984;    // set to default couchdb port
    }

    m_couchDbStruct.port = dbPort;
    m_couchDbStruct.dbName = value("dbName").toString();

    endGroup();

#ifdef DEBUG_MODE
    qDebug() << "AGATHA SERVER PORT: " << m_serverConfigStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.ip;
    qDebug() << "COUCHDB : " << m_couchDbStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.dbName;
#endif

    CliErrorReporter::printNotification("[INFO] Done!");
}

