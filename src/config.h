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
class QNetworkReply;

class Config : public QSettings
{
    Q_OBJECT

public:
    /** struct with server settigns */
    struct ServerConfigStruct
    {
        quint16 port;
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
            return url;
        }
    };

    /** Constructor
     * @param customConfig custom config file path
     */
    Config(const QString &configFile = QString("AgathaServerCfg.cfg"), QObject *parent = 0);
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
    QNetworkReply *m_networkReply;

    QList<QString> m_allowedDbs;                        /** list of allowed databases to create/use */
};



#endif  // CONFIG_H