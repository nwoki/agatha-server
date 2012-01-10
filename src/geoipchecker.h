/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef GEOIPCHECKER_H
#define GEOIPCHECKER_H

#include <QtSql/QSqlDatabase>

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

    ///TODO method for checking location given an ip

private:

};


#endif  // GEOIPCHECKER_H