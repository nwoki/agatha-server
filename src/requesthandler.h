/*
 * requesthandler.cpp
 *
 * This file is part of Agatha
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QtCore/QStringList>

/**
 * Class that handles the RESTFUL requests parsed from the webservice
 */

class RequestHandler
{

public:
    RequestHandler();
    ~RequestHandler();

    void handleGetRequest(const QStringList &responseParts);
    void handlePostRequest(const QStringList &responseParts);
    void handlePutRequest(const QStringList &responseParts);

private:

};

#endif // REQUESTHANDLER_H
