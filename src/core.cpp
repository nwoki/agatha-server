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
#include "webservice.h"

Core::Core(const QString &customConfigArg, QObject *parent)
    : QObject(parent)
    , m_config(new Config(customConfigArg, this))
    , m_webservice(0)
{
    // connect config ready signal and start the server.
    connect(m_config, SIGNAL(ready()), this, SLOT(onConfigReady()));

}

Core::~Core()
{
}


void Core::onConfigReady()
{
    qDebug("[Core::onConfigReady]");

    // start server
    m_webservice = new WebService(m_config->serverConfigStruct(), this);
}
