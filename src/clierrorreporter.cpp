/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "clierrorreporter.h"

#include <QtCore/QDebug>

#define BOLD_GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"
#define RED "\e[0;31m"
#define BOLD_RED "\e[1;31m"
#define COLOR_END "\e[0m"

void CliErrorReporter::printError(CliErrorReporter::ErrorType type, CliErrorReporter::ErrorLevel lvl, const QString& errMsg)
{
    QString cliMsg;

    // set colour
    if (lvl == WARNING) {
        cliMsg.append(YELLOW);
    } else if (lvl == ERROR) {
        cliMsg.append(RED);
    } else if (lvl == CRITICAL) {
        cliMsg.append(BOLD_RED);
    }

    if (type == APPLICATION) {
        cliMsg.append("[AgathaServer:Application] : ");
    } else if (type == DATABASE) {
        cliMsg.append("[AgathaServer:Database] : ");
    }

    // add error message
    cliMsg.append(errMsg);

    // close colour
    cliMsg.append(COLOR_END);

    // print
    qWarning() << cliMsg;
}

void CliErrorReporter::printNotification(const QString& msg)
{
    QString notification(BOLD_GREEN);

    notification.append(msg);
    notification.append(COLOR_END);

    qWarning() << notification;
}

