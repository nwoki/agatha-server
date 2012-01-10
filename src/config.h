/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QSettings>

/**
 * Class used to fetch config data for the server settings
 * @author Francesco Nwokeka
 */

class Config : public QSettings
{
    Q_OBJECT

public:
    /** struct with geoip mysql database info */
    struct GeoIpConfigStruct
    {
        QString dbName;
        QString user;
        QString password;
    };

    /// TODO
//     /** struct with agatha couchDb info */
//     struct CouchDbStruct
//     {
//
//     };

    Config(QObject *parent = 0);
    ~Config();

    GeoIpConfigStruct geoipConfigStruct() const;    /** returns MySqlConfigStruct with mysql config values */

private:
    void loadConfigFile();                          /** load config file values */

    GeoIpConfigStruct m_geoIpConfigStruct;
};

#endif  // CONFIG_H