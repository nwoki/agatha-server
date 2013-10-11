/*
 * config.h
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QDebug>
#include <QtCore/QSettings>

/**
 * Class used to fetch config data for the server settings
 * @author Francesco Nwokeka
 */

class QNetworkAccessManager;

class Config : public QSettings
{
    Q_OBJECT

public:
    /** struct with server settings */
    struct ServerConfigStruct
    {
        quint16 port;
    };

    /** struct with agatha couchDb info */
    struct CouchDbStruct
    {
        QString ip;
        quint16 port;
        QString dbName;

        /** returns url as ip:port/dbName/ for queries to the database */
        QString queryUrl()
        {
            QString url("http://");

            url += ip + ":" + QString::number(port) + "/" + dbName + "/";
            return url;
        }
    };

    /** Constructor
     * @param customConfig custom config file path
     */
    Config(QNetworkAccessManager *netManager, const QString &configFile = QString("AgathaServerCfg.cfg"), QObject *parent = 0);
    ~Config();

    CouchDbStruct couchDbStruct() const;                /** returns CouchDbStruct with couchDB settings */
    ServerConfigStruct serverConfigStruct() const;      /** returns ServerConfigStruct with general server settings */

Q_SIGNALS:
    void ready();                                       /** emitted when config object has validated all settings (db, server port etc)*/

private Q_SLOTS:
    void onCheckCouchDbReceived();
    void onCreateDbReceived();
    void onNetworkReplyReceived();

private:
    void checkCouchDb();                                /** checks that we can communicate with the couch db */
    void loadConfigFile();                              /** load config file values */

    /**
     * checks for existance of the couch database. If it's not there
     * we attemp to create it.
     *
     * @returns result of the presence of the database
     */
    bool prepareCouchDb();

    CouchDbStruct m_couchDbStruct;
    ServerConfigStruct m_serverConfigStruct;

    QNetworkAccessManager *m_netManager;

    /**
     * list of allowed game databases agatha supports. These values are hardcoded.
     * A check is made upon configuration loading. If the user specifies a database that's not supported,
     * the agatha-server node fails to start
     */
    QList<QString> m_allowedDbs;
};



#endif  // CONFIG_H
