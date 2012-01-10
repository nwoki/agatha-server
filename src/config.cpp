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


void Config::loadConfigFile()
{
    beginGroup("geoipDb");
    m_geoIpConfigStruct.dbName = value("dbName").toString();
    m_geoIpConfigStruct.password = value("password").toString();
    m_geoIpConfigStruct.user = value("user").toString();
    endGroup();
}

