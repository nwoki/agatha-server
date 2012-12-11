/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "../clierrorreporter.h"
#include "serverauthchecker.h"

#include <QtNetwork/QHostInfo>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

ServerAuthChecker::ServerAuthChecker()
    : QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "agathaDb"))
{
    QHostInfo agathaAuthServer = QHostInfo::fromName("www.agathaproject.org");

    // setup database connection
    setHostName("localhost"/*agathaAuthServer.addresses().at(0).toString()*/);
//     setPort(6666);

    /// TODO when aquilinux adds the new database and user
    setDatabaseName("agatha");
    setUserName("root"/*"srvAgatha"*/);
    setPassword("root"/*"cicciopuzza"*/);

    if (!openDatabase()) {
        std::exit(1);
    }
}

ServerAuthChecker::~ServerAuthChecker()
{
}

bool ServerAuthChecker::isTokenValid(const QString &serverIp, quint16 serverPort, const QString& serverToken)
{
    bool result = false;

    if (!isOpen()) {
        if (!openDatabase()) {
            return result;
        }
    }

    /// TODO do the sql query and return if there is a record on db or not
    QString queryStr("select clan from agatha_servers where ip='");
    queryStr.append(serverIp);
    queryStr.append("' and port='");
    queryStr.append(QString::number(serverPort));
    queryStr.append("' and token='");
    queryStr.append(serverToken);
    queryStr.append("';");

    QSqlQuery query(database("agathaDb"));

    if (!query.exec(queryStr)) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE, CliErrorReporter::ERROR, query.lastError().text());
    } else if (query.size() == 0 || query.size() == -1) {
        return false;
    } else if (query.next()) {
        result = true;
    }

#ifdef DEBUG_MODE
    qDebug() << "[ServerAuthChecker::isTokenValid] QUERY STRING IS: " << queryStr;
    qDebug() << "[ServerAuthChecker::isTokenValid] CLAN ID IS: " << result;
#endif

    return result;
}

bool ServerAuthChecker::openDatabase()
{
    bool isDbOpen;

    /// TODO need to log database errors to file!
    if (!open()) {
        QString errMsg("Can't open connection to webAgatha database.\n Error is: ");
        errMsg.append(lastError().text());

        CliErrorReporter::printError(CliErrorReporter::DATABASE
        , CliErrorReporter::CRITICAL
        , errMsg);
        isDbOpen = false;
    } else {
        CliErrorReporter::printNotification("Connected to mysql server: agatha..");
        isDbOpen = true;
    }

    return isDbOpen;
}

