/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef COMMANDEXECUTER_H
#define COMMANDEXECUTER_H

#include <QString>
#include <QVariantMap>

/**
 * Class used for executing incoming commands from various bots around the world!
 * This class interacts with Agatha's couchDB database
 * @author Francesco Nwokeka
 */

class GeoIpChecker;

class CommandExecuter
{
public:
    CommandExecuter();
    ~CommandExecuter();

    /**
     * execute given command for the game specified using info in the player VariantMap
     * @param command query type to execute on database
     * @param game gametype to execute query for
     * @param player QVariantMap with info on the player to use in query
     */
    void execute(const QString &command, const QString &game, const QVariantMap &player);

private:
    GeoIpChecker *m_geoIpChecker;
};



#endif  // COMMANDEXECUTER_H