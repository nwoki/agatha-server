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
}

Config::ServerConfigStruct Config::serverConfigStruct() const
{
    return m_serverConfigStruct;
}


void Config::loadConfigFile()
{
    bool ok;

    beginGroup("server");
    int port = value("port").toInt(&ok);

    if (!ok) {
        CliErrorReporter::printError(CliErrorReporter::APPLICATION, CliErrorReporter::WARNING, "Invalid port. Using default (1337)");
        port = 1337;
    }

    m_serverConfigStruct.port = port;
    endGroup();

//     DEBUG
#ifdef DEBUG_MODE
    qDebug() << "SERVER : " << m_serverConfigStruct.port;
#endif
}

