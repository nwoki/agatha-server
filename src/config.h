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
public:
    /** struct with server settigns */
    struct ServerConfigStruct
    {
        int port;
    };

    /** struct with agatha couchDb info */
    struct CouchDbStruct
    {
        QString ip;
        int port;
        QString dbName;

        /** returns url asd ip:port/dbName/ for queries to the database */
        QString queryUrl()
        {
            QString url("http://");

            url += ip + ":" + QString::number(port) + "/" + dbName + "/";
//             url.append(ip);
//             url.append(":");
//             url.append(QString::number(port));
//             url.append("/");
//             url.append(dbName);
//             url.append("/");

            return url;
        }
    };

    /** Constructor
     * @param customConfig custom config file path
     */
    Config(const QString &configFile = QString("AgathaServerCfg.cfg"), QObject *parent = 0);
    ~Config();

    CouchDbStruct couchDbStruct() const;            /** returns CouchDbStruct with couchDB settings */
    ServerConfigStruct serverConfigStruct() const;  /** returns ServerConfigStruct with general server settings */

private:
    void loadConfigFile();                          /** load config file values */

    CouchDbStruct m_couchDbStruct;
    ServerConfigStruct m_serverConfigStruct;
};

#endif  // CONFIG_H