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

    /// TODO
//     /** struct with agatha couchDb info */
//     struct CouchDbStruct
//     {
//
//     };
    /** Constructor
     * @param customConfig custom config file path
     */
    Config(const QString &configFile = QString("AgathaServerCfg.cfg"), QObject *parent = 0);
    ~Config();

    ServerConfigStruct serverConfigStruct() const;  /** returns ServerConfigStruct with general server settings */

private:
    void loadConfigFile();                          /** load config file values */

    ServerConfigStruct m_serverConfigStruct;
};

#endif  // CONFIG_H