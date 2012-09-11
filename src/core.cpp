/*
 * core.cpp
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "commandexecuter.h"
#include "config.h"
#include "core.h"
#include "server.h"

Core::Core(const QString &customConfigArg, QObject *parent)
    : QObject(parent)
    , m_config(new Config(customConfigArg))
    , m_server(0)
{
    // connect config ready signal and start the server.
    connect(m_config, SIGNAL(ready()), this, SLOT(onConfigReady()));

}

Core::~Core()
{
    m_config->deleteLater();
    m_server->deleteLater();
}


void Core::onConfigReady()
{
#ifdef DEBUG_MODE
    qDebug("[Core::onConfigReady]");
#endif

    // initialize server
    m_server = new Server(m_config->serverConfigStruct(), new CommandExecuter(m_config->couchDbStruct()), this);
}
