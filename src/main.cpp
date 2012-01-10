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

    /// TODO load config values from file
    Config *agathaConfig = new Config();

    /// TODO start server
    Server srvAgatha(agathaConfig);

    return app->exec();
}