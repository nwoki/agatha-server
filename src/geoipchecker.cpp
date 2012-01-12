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

#include <QtCore/QDebug>
#include <QtSql/QSqlError>

GeoIpChecker::GeoIpChecker(Config* config)
    : QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))
{
    // setup database connection
    setHostName("localhost");   /// TODO use remote mysql server
    setDatabaseName(config->geoipConfigStruct().dbName);
    setUserName(config->geoipConfigStruct().user);
    setPassword(config->geoipConfigStruct().password);

    if (!openDatabase()) {
        std::exit(1);
    }
}

GeoIpChecker::~GeoIpChecker()
{
    close();
}

QString GeoIpChecker::location(const QString& ip)
{
    // be sure we can use the database
    if (!isOpen()) {
        if (!openDatabase()) {
            /// TODO add timestamp of when error occured
            return QString("Unknown - Agatha Database Error");
        }
    }

    // generate values for mysql query
    QStringList ipValues = ip.split(".", QString::SkipEmptyParts);
    int ipNum = genIpNum(ipValues);
    QSqlQuery query;

    // get locID from database
    return queryForLocName(query, queryForLocId(query, ipNum));
}

int GeoIpChecker::genIpNum(const QStringList& splitIp)
{
    bool ok;

    /// TODO I should check that the conversion is always correct but can't be bothered
    int w = splitIp.at(0).toInt(&ok, 10);
    int x = splitIp.at(1).toInt(&ok, 10);
    int y = splitIp.at(2).toInt(&ok, 10);
    int z = splitIp.at(3).toInt(&ok, 10);

#ifdef DEBUG_MODE
    qDebug() << "split ip numbers are : " << w << x << y << z;
#endif

    // transform
    return ((w*16777216) + (65536*x) + (256*y) + (z));;
}

bool GeoIpChecker::openDatabase()
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

QString GeoIpChecker::queryForLocId(QSqlQuery &query, int ipNum)
{
    QString result;
    QString queryStr("SELECT locID FROM Blocks WHERE ");
    queryStr.append(QString::number(ipNum));
    queryStr.append(" >= startIPNum AND ");
    queryStr.append(QString::number(ipNum));
    queryStr.append(" <= endIPNum LIMIT 1;");

    if (!query.exec(queryStr)) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE, CliErrorReporter::ERROR, query.lastError().text());
    } else {
        while (query.next()) {
            // just need 1st record as we limited the response
            result = query.value(0).toString();
        }
    }

#ifdef DEBUG_MODE
    qDebug() << "QUERY STRING IS: " << queryStr;
    qDebug() << "LOC ID IS: " << result;
#endif

    return result;
}

QString GeoIpChecker::queryForLocName(QSqlQuery &query, const QString &locId)
{
    /// TODO we can select a lot more info than just the city name!!
    QString result;
    QString queryStr("select city from Location where locID=");
    queryStr.append(locId);
    queryStr.append(";");

    if (!query.exec(queryStr)) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE, CliErrorReporter::ERROR, query.lastError().text());
    } else {
        while (query.next()) {
            result = query.value(0).toString();
        }
    }

#ifdef DEBUG_MODE
    qDebug() << "QUERY STRING IS: " << queryStr;
    qDebug() << "LOC NAME IS: " << result;
#endif

    return result;
}


