/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"
#include "config.h"
#include "geoipchecker.h"

#include <QtSql/QSqlError>

GeoIpChecker::GeoIpChecker(Config* config)
    : QSqlDatabase("QMYSQL")
{
    // setup database connection
    setHostName("localhost");
    setDatabaseName(config->geoipConfigStruct().dbName);
    setUserName(config->geoipConfigStruct().user);
    setPassword(config->geoipConfigStruct().password);

    if (!open()) {
        QString errMsg("Can't open connection to GeoIp database.\n Error is: ");
        errMsg.append(lastError().text());

        CliErrorReporter::printError(CliErrorReporter::DATABASE
                                    , CliErrorReporter::CRITICAL
                                    , errMsg);
        std::exit(1);
    } else {
        CliErrorReporter::printNotification("Connected to mysql server..");
    }
}

GeoIpChecker::~GeoIpChecker()
{
    close();
}
