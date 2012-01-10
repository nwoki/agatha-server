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

Config::Config(QObject* parent)
    : QSettings("AgathaServerCfg.cfg", QSettings::IniFormat, parent)
{
    // be sure config file exists
    QFile configFile("AgathaServerCfg.cfg");

    if (!configFile.exists()) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION
                                    , CliErrorReporter::CRITICAL
                                    , "Can't find config file in current directory");
        /// TODO define different error status?
        std::exit(0);
    }

    // load data
    loadConfigFile();
}

Config::~Config()
{

}

Config::GeoIpConfigStruct Config::geoipConfigStruct() const
{
    return m_geoIpConfigStruct;
}

Config::ServerConfigStruct Config::serverConfigStruct() const
{
    return m_serverConfigStruct;
}


void Config::loadConfigFile()
{
    beginGroup("server");
    m_serverConfigStruct.authServer = value("authServer").toString();
    m_serverConfigStruct.authToken = value("authToken").toString();
    m_serverConfigStruct.port = value("port").toInt();
    endGroup();

    beginGroup("geoipDb");
    m_geoIpConfigStruct.dbName = value("dbName").toString();
    m_geoIpConfigStruct.password = value("password").toString();
    m_geoIpConfigStruct.user = value("user").toString();
    endGroup();

//     DEBUG
    qDebug() << "SERVER : " << m_serverConfigStruct.authServer
                            << m_serverConfigStruct.authToken
                            << m_serverConfigStruct.port;

    qDebug() << "GEOIP : "  << m_geoIpConfigStruct.dbName
                            << m_geoIpConfigStruct.password
                            << m_geoIpConfigStruct.user;
}

