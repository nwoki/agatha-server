/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "config.h"
#include "commandexecuter.h"
#include "server.h"

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString customConfig(argv[1]);
    Config *agathaConfig;

    // check if user set a custom config file
    if (customConfig.isEmpty()) {
        agathaConfig = new Config();
    } else {
        agathaConfig = new Config(customConfig);
    }

    CommandExecuter *cmdExe = new CommandExecuter(agathaConfig->couchDbStruct());

    Server srvAgatha(agathaConfig->serverConfigStruct(), cmdExe);

    // don't need this anymore
    agathaConfig->deleteLater();

    return app.exec();
}