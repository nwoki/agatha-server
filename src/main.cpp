/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"
#include "config.h"
#include "server.h"

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication *app = new QCoreApplication(argc, argv);

    QString customConfig(argv[1]);
    Config *agathaConfig;

    // check if user set a custom config file
    if (customConfig.isEmpty()) {
        agathaConfig = new Config();
    } else {
        agathaConfig = new Config(customConfig);
    }

    Server srvAgatha(agathaConfig);

    return app->exec();
}