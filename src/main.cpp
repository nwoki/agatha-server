/*
 * main.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012-2014 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */


#include "core.h"
#include "webservice.h"

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QString customConfig(argv[1]);

    /**
     * The Starter object starts everything. After checking that all is ok (config),
     * it starts the Server. Otherwise it exits
     */
    Core core(customConfig);

//     CommandExecuter *cmdExe = new CommandExecuter(agathaConfig->couchDbStruct());

    return app.exec();
}
