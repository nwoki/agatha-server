/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"
#include "config.h"
#include "stdlib.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>

Config::Config(const QString &configFile, QObject* parent)
    : QSettings(configFile, QSettings::IniFormat, parent)
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
    } else {
        CliErrorReporter::printNotification("Loading configuration file..");
    }

    // load data
    loadConfigFile();
}


Config::~Config()
{
    qDebug("[Config::~Config]");
}


Config::CouchDbStruct Config::couchDbStruct() const
{
    return m_couchDbStruct;
}


Config::ServerConfigStruct Config::serverConfigStruct() const
{
    return m_serverConfigStruct;
}


void Config::loadConfigFile()
{
    bool ok;

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

//     DEBUG
#ifdef DEBUG_MODE
    qDebug() << "AGATHA SERVER PORT: " << m_serverConfigStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.ip;
    qDebug() << "COUCHDB : " << m_couchDbStruct.port;
    qDebug() << "COUCHDB : " << m_couchDbStruct.dbName;
#endif
}

