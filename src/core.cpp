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

#include <QtNetwork/QNetworkAccessManager>

Core::Core(const QString &customConfigArg, QObject *parent)
    : QObject(parent)
    , m_netManager(new QNetworkAccessManager(this))
    , m_config(0)
    , m_webservice(0)
{
    m_config = new Config(m_netManager, customConfigArg, this);

    // connect config ready signal and start the server.
    connect(m_config, &Config::ready, [this]() {
        m_webservice = new WebService(m_config->couchDbStruct(), m_netManager, m_config->serverConfigStruct().port, this);
    });
}

Core::~Core()
{
}
