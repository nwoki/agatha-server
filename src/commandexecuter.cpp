/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "commandexecuter.h"
#include "checkers/geoipchecker.h"

#include <QtCore/QDebug>

CommandExecuter::CommandExecuter()
    : m_geoIpChecker(new GeoIpChecker)
{
}

CommandExecuter::~CommandExecuter()
{
    delete m_geoIpChecker;
}

void CommandExecuter::execute(const QString &command, const QString &game, const QVariantMap &player)
{
    ///TODO add geoloc position and timestamp before sending to couchDB

    qDebug() << "CommandExecuter::execute " << command << " for " << game;
    qDebug() << "Count " << player.count();

    qDebug() << player["nick"].toString();
    qDebug() << player["gear"].toString();
    qDebug() << player["ip"].toString();
    qDebug() << player["weaponMode"].toString();
    qDebug() << player["guid"].toString();

    /// TEST
    qDebug() << "PLAYER IP: " << player["ip"].toString() << " LOCATED @ " << m_geoIpChecker->location(player["ip"].toString());
}