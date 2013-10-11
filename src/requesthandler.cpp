/*
 * requesthandler.cpp
 *
 * This file is part of <project name>
 * Copyright (C) 2012-2013 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "clierrorreporter.h"
#include "commandexecuter.h"
#include "requesthandler.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>

#include <QtNetwork/QTcpSocket>


RequestHandler::RequestHandler(Config::CouchDbStruct couchDbStruct, QNetworkAccessManager *netManager, QObject *parent)
    : QObject(parent)
    , m_commandExecuter(new CommandExecuter(couchDbStruct, netManager, this))
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


void RequestHandler::handleHttpRequest(const QByteArray &json, QTcpSocket *httpSocket)
{
    qDebug("[RequestHandler::handleHttpRequest]");
    qDebug() << "[RequestHandler::handleHttpRequest] json to handle: " << json;

    QJsonObject jsonObj = jsonObject(json);

    if (jsonObj.isEmpty()) {
        qDebug("[RequestHandler::handleHttpRequest] ERROR: json obj is empty");
        CliErrorReporter::printError(CliErrorReporter::NETWORK, CliErrorReporter::WARNING, "[RequestHandler::handleHttpRequest] json obj is empty");

        // send error msg and kill socket
        // TODO error response
        httpSocket->close();
        httpSocket->deleteLater();
        return;
    }

    CommandExecuter::Command cmd = command(jsonObj);

    // TODO send correct data. For now stud data is used for prototyping and getting code done
    m_commandExecuter->execute(cmd, jsonObj.value("token").toString(), jsonObj.value("game").toString(), QVariantMap(), httpSocket);
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
