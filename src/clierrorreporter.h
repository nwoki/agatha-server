/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#ifndef CLIERRORREPORTER_H
#define CLIERRORREPORTER_H

#include <QtCore/QString>

/**
 * Class used to print error messages on the console during runtime
 * @author Francesco Nwokeka
 */

class CliErrorReporter
{
public:
    enum ErrorType {
        APPLICATION,    /** application related error */
        DATABASE,       /** database related error */
        NETWORK         /** network related error */
    };

    enum ErrorLevel {
        WARNING,        /** simple warning (yellow) */
        ERROR,          /** an error that doesn't compromise program stability (red) */
        CRITICAL        /** system crash/stability compromised. (bold red) */
    };

    /** print error to screen. According to the error lvl, prints will have different colours
     * @param type error type
     * @param lvl error level
     * @param errMsg error message to display
     */
    static void printError(ErrorType type, ErrorLevel lvl, const QString &errMsg);

    /** print notification message to screen. Colour = green */
    static void printNotification(const QString &msg);
};



#endif  // CLIERRORREPORTER_H