/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "../clierrorreporter.h"
#include "geoipchecker.h"

#include <QtCore/QDebug>
#include <QtNetwork/QHostInfo>
#include <QtSql/QSqlError>

GeoIpChecker::GeoIpChecker()
    : QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "geoipDb"))
{
    QHostInfo agathaAuthServer = QHostInfo::fromName("www.agathaproject.org");

    /// TODO crash if there is no internet connection. Check for size of addresses.count() > 0

    // setup database connection
    setHostName(agathaAuthServer.addresses().at(0).toString());
    setPort(6666);
    setDatabaseName("geoip");
    setUserName("srvAgatha");
    setPassword("cicciopuzza");

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
    QSqlQuery query(database("geoipDb"));

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
        CliErrorReporter::printNotification("Connected to mysql server: geoip..");
        isDbOpen = true;
    }

    return isDbOpen;
}

QString GeoIpChecker::queryForLocId(QSqlQuery &query, int ipNum)
{
    QString result;
    QString queryStr("SELECT locID FROM OrderedBlocks WHERE ");
    queryStr.append(QString::number(ipNum));
    queryStr.append(" >= startIPNum LIMIT 1;");

    if (!query.exec(queryStr)) {
        CliErrorReporter::printError(CliErrorReporter::DATABASE, CliErrorReporter::ERROR, query.lastError().text());
    } else {
        while (query.next()) {
            // just need 1st record as we limited the response
            result = query.value(0).toString();
        }
    }

#ifdef DEBUG_MODE
    qDebug() << "[GeoIpChecker::queryForLocId] QUERY STRING IS: " << queryStr;
    qDebug() << "[GeoIpChecker::queryForLocId] LOC ID IS: " << result;
#endif

    return result;
}

QString GeoIpChecker::queryForLocName(QSqlQuery &query, const QString &locId)
{
    if (locId.isEmpty()) {
        return QString("Unknown");
    }

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
    qDebug() << "[GeoIpChecker::queryForLocId] QUERY STRING IS: " << queryStr;
    qDebug() << "[GeoIpChecker::queryForLocId] LOC NAME IS: " << result;
#endif

    return result;
}


