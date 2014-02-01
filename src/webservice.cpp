/*
 * webservice.cpp
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2014 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */


#include "commandexecuter.h"
#include "requesthandler.h"
#include "webservice.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

#include <restful/includes/Collection>
#include <restful/includes/Server>

WebService::WebService(Config::CouchDbStruct couchDbStruct, quint16 port, QObject *parent)
    : m_server(new RESTFul::Server(port))
{
    qDebug("[WebService::WebService]");

    // set config info
    m_serverConfigStruct.port = port;

    // prepare api
    RESTFul::Collection *testColl = new RESTFul::Collection("/test");
    m_server->addCollection(testColl);
}

WebService::~WebService()
{
}

void WebService::setupConnects()
{
    qDebug("[WebService::setupConnects]");

}
