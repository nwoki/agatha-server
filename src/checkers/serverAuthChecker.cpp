/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "../clierrorreporter.h"
#include "serverAuthChecker.h"

#include <QtNetwork/QHostInfo>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

ServerAuthChecker::ServerAuthChecker()
    : QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))
{
    QHostInfo agathaAuthServer = QHostInfo::fromName("www.agathaproject.org");

    // setup database connection
    setHostName(agathaAuthServer.addresses().at(0).toString());
    setPort(6666);

    /// TODO when aquilinux adds the new database and user
//     setDatabaseName("geoip");
//     setUserName("srvAgatha");
//     setPassword("cicciopuzza");

    if (!openDatabase()) {
        std::exit(1);
    }
}

ServerAuthChecker::~ServerAuthChecker()
{
}

bool ServerAuthChecker::isTokenValid(const QString& serverToken)
{
    /// TODO do the sql query and return if there is a record on db or not
    QString queryStr("select id from servers where token=");
    queryStr.append(serverToken);

    QSqlQuery query;
    bool result = false;

    if (!query.exec(queryStr)) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE, CliErrorReporter::ERROR, query.lastError().text());
    } else {
        if (query.next()) {
            result = true;
        }
    }

#ifdef DEBUG_MODE
    qDebug() << "QUERY STRING IS: " << query.value(0).toString();
    qDebug() << "LOC ID IS: " << result;
#endif

    return result;
}

bool ServerAuthChecker::openDatabase()
{
    bool isDbOpen;

    /// TODO need to log database errors to file!
    if (!open()) {
        QString errMsg("Can't open connection to GeoIp database.\n Error is: ");
        errMsg.append(lastError().text());

        CliErrorReporter::printError(CliErrorReporter::DATABASE
        , CliErrorReporter::CRITICAL
        , errMsg);
        isDbOpen = false;
    } else {
        CliErrorReporter::printNotification("Connected to mysql server..");
        isDbOpen = true;
    }

    return isDbOpen;
}

