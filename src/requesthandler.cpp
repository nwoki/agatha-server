/*
 * requesthandler.cpp
 *
 * This file is part of <project name>
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "clierrorreporter.h"
#include "requesthandler.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>


RequestHandler::RequestHandler(QObject *parent)
    : QObject(parent)
{
}


RequestHandler::~RequestHandler()
{
}


CommandExecuter::Command RequestHandler::command(const QJsonObject &jsonObj)
{
    qDebug("[RequestHandler::command]");

    QString command = jsonObj.value("command").toString();

    qDebug() << "[Server::determineCommand] command is: " << command.toLower();

    if (command.toLower() == "add") {
        return CommandExecuter::ADD;
    } else if (command.toLower() == "ban") {
        return CommandExecuter::BAN;
    } else if (command.toLower() == "isbanned") {
        return CommandExecuter::IS_BANNED;
    } else if (command.toLower() == "whois") {
        return CommandExecuter::WHO_IS;
    } else {
        return CommandExecuter::INVALID_COMMAND;
    }
}


void RequestHandler::handleGetRequest(const QByteArray &json)
{
    qDebug("[RequestHandler::handleGetRequest]");
    qDebug() << "[RequestHandler::handlePostRequest] json to handle: " << json;

    QJsonObject jsonObj = jsonObject(json);

    if (jsonObj.isEmpty()) {
        return;
    }

    CommandExecuter::Command cmd = command(jsonObj);
}


void RequestHandler::handlePostRequest(const QByteArray &json)
{
    qDebug("[RequestHandler::handlePostRequest]");
    qDebug() << "[RequestHandler::handlePostRequest] json to handle: " << json;

    QJsonObject jsonObj = jsonObject(json);

    if (jsonObj.isEmpty()) {
        return;
    }

    CommandExecuter::Command cmd = command(jsonObj);
}


void RequestHandler::handlePutRequest(const QByteArray &json)
{
    qDebug("[RequestHandler::handlePutRequest]");
    qDebug() << "[RequestHandler::handlePostRequest] json to handle: " << json;

    QJsonObject jsonObj = jsonObject(json);

    if (jsonObj.isEmpty()) {
        return;
    }

    CommandExecuter::Command cmd = command(jsonObj);
}


QJsonObject RequestHandler::jsonObject(const QByteArray &jsonData)
{
    qDebug("[RequestHandler::jsonObject]");

    QJsonParseError jsonError;
    QJsonDocument jsonParser = QJsonDocument::fromJson(jsonData, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        QString errStr("[RequestHandler::jsonObject] Invalid json!");
        errStr += " - " + jsonError.errorString();

        CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::ERROR, errStr);
        return QJsonObject();
    }

    // extract json object
    return jsonParser.object();
}
