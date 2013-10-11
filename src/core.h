/*
 * core.h
 *
 * This file is part of agathaServer
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef CORE_H
#define CORE_H


#include <QtCore/QObject>

/**
 * @brief Class used to hold all together and control the flow of the application
 * @author Francesco Nwokeka
 */

class Config;
class WebService;
class QNetworkAccessManager;

class Core : public QObject
{
    Q_OBJECT

public:
    Core(const QString &customConfigArg = QString(), QObject *parent = 0);
    ~Core();

private:
    QNetworkAccessManager *m_netManager;
    Config *m_config;
    WebService *m_webservice;
};

#endif // CORE_H
