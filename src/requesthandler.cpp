/*
 * requesthandler.cpp
 *
 * This file is part of <project name>
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "requesthandler.h"


RequestHandler::RequestHandler(QObject *parent)
    : QObject(parent)
{
    qDebug("[RequestHandler::RequestHandler]");
}


RequestHandler::~RequestHandler()
{
}


void RequestHandler::handleGetRequest(const QStringList &responseParts)
{
    qDebug("[RequestHandler::handleGetRequest]");
}


void RequestHandler::handlePostRequest(const QStringList &responseParts)
{
    qDebug("[RequestHandler::handlePostRequest]");
}


void RequestHandler::handlePutRequest(const QStringList &responseParts)
{
    qDebug("[RequestHandler::handlePutRequest]");
}


