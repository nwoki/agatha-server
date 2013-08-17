/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef SERVERAUTHCHECKER_H
#define SERVERAUTHCHECKER_H

#include <QtCore/QObject>

// DEPRECATED

/**
 * Class used to determine if a server is associated with the Agatha service. This is done
 * by checking if the given server token is stored on the Agatha "servers" database
 * @author Francesco Nwokeka
 */

class ServerAuthChecker : public QObject
{
    Q_OBJECT

public:
    ServerAuthChecker(QObject *parent = 0);
    ~ServerAuthChecker();

    bool isTokenValid(const QString &serverIp, const QString &serverToken);

private:
//     /** opens a connection to the database. Returns connection status */
//     bool openDatabase();
};

#endif  // SERVERAUTHCHECKER_H