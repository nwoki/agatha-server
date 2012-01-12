/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef GEOIPCHECKER_H
#define GEOIPCHECKER_H

#include <QtCore/QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class Config;

/**
 * Class used to fetch information from the GeoIp mysql database
 * @author Francesco Nwokeka
 */

class GeoIpChecker : public QSqlDatabase
{
public:
    GeoIpChecker(Config *config);
    ~GeoIpChecker();

    /** retrieve location from given ip
     * @param ip ip address of client to lookup
     */
    QString location(const QString &ip);

private:
    /** generates an ipNum for querying the database */
    int genIpNum(const QStringList &splitIp);

    /** opens a connection to the database. Returns connection status */
    bool openDatabase();

    /** queries database for a location id given an ip
     * @param query query object
     * @param ipNum converted ip string to ipNum via the genIpNum method
     */
    QString queryForLocId(QSqlQuery &query, int ipNum);

    /** queries database for a location name given a location id
     * @param query query object
     * @param locId location id to do query on
     */
    QString queryForLocName(QSqlQuery &query, const QString &locId);

};


#endif  // GEOIPCHECKER_H