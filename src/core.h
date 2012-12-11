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
 * Class used to hold all together and control the flow of the application
 * @author Francesco Nwokeka
 */

class Config;
class WebService;

class Core : public QObject
{
    Q_OBJECT

public:
    Core(const QString &customConfigArg = QString(), QObject *parent = 0);
    ~Core();

private Q_SLOTS:
    void onConfigReady();       /** called when config class has finished verifying params and databases */

private:
    Config *m_config;
    WebService *m_webservice;
};

#endif // CORE_H
