/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include <QtGui/QApplication>

#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog *dialog = new Dialog();

    return app.exec();
}