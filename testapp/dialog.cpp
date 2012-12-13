/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "dialog.h"
#include "ui_dialog.h"

#include "../src/qjson/include/QJson/Serializer"

#include <QtCore/QDateTime>
#include <QtCore/QVariantList>

#include <QtGui/QAbstractButton>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

Dialog::Dialog(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::Main)
    , m_socket(new QUdpSocket)
    , m_reply(0)
    , m_netManager(new QNetworkAccessManager(this))
{
    m_ui->setupUi(this);

    QStringList commands;
    commands << "add" << "ban" << "isBanned" << "whoIs";
    m_ui->commandCombo->addItems(commands);

    m_ui->gametypeCombobox->addItem("UrbanTerror 4.1.x");

    m_socket->bind(QHostAddress::Any, 1234);

    setupSignalsAndSlots();
    show();
}


Dialog::~Dialog()
{
    delete m_ui;
}


QVariantMap Dialog::createPlayerMap()
{
    QVariantMap player;

    player.insert("nick", m_ui->playerNickLineEdit->text());
    player.insert("ip", m_ui->playerIpLineEdit->text());
    player.insert("gear", m_ui->playerGearLineEdit->text());
    player.insert("weaponMode", m_ui->playerWeaponModeLineEdit->text());
    player.insert("guid", m_ui->guidLineEdit->text());

    return player;
}


QString Dialog::gametype() const
{
    qDebug("[Dialog::gametype]");

    QString gametype;
    int gametypeIndex = m_ui->gametypeCombobox->currentIndex();

    if (gametypeIndex == 0) {                   // URBAN TERROR 4.1.x
        gametype += "/urbanterror4_1_x";
    }

    return gametype;
}


void Dialog::onButtonBoxClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton butt = m_ui->buttonBox->standardButton(button);

    if (butt == QDialogButtonBox::Close) {
        QApplication::quit();
    } else if (butt == QDialogButtonBox::Reset) {
        m_ui->textEdit->clear();
    } else if (butt == QDialogButtonBox::Ok) {
        sendPacketToServer();
    }
}

void Dialog::onEditingFinished()
{
    // disconnect from previous host
    m_socket->disconnectFromHost();
}

void Dialog::onReadyRead()
{
    QByteArray incoming;
    incoming.resize(m_socket->pendingDatagramSize());
    qDebug() << "READY READ: " << m_socket->readDatagram(incoming.data(), incoming.size());
    qDebug() << "DATA: " << incoming;
}

void Dialog::onPreviewButtonClicked()
{
    if (!m_ui->textEdit->toPlainText().isEmpty()) {
        m_ui->textEdit->clear();
    }

    m_ui->textEdit->setText(prepareMessage());
}


void Dialog::onReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[Dialog::onReplyError]");

    qDebug() << m_reply->errorString();
}


void Dialog::onReplyReadyRead()
{
    qDebug("[Dialog::onReplyReadyRead]");

    qDebug() << m_reply->readAll();
}


void Dialog::onSocketError(QAbstractSocket::SocketError err)
{
    // create error message and show Error Tab
    QString errorMsg;
    errorMsg.append(QTime::currentTime().toString("<b>SocketError@HH:mm</b>"));
    errorMsg.append(" - ");
    errorMsg.append(m_socket->errorString());

    // add error message to error tab and change focus
    m_ui->errorTextEdit->append(errorMsg);
    m_ui->tabWidget->setCurrentIndex(1);
}

QByteArray Dialog::prepareMessage()
{
    QVariantMap preview;
    preview.insert("token", m_ui->tokenLineEdit->text());
    preview.insert("game", "URT_411");
    preview.insert("command", m_ui->commandCombo->currentText());
    preview.insert("playerInfo", createPlayerMap());

    QJson::Serializer serializer;
    return serializer.serialize(preview);
}

QString Dialog::prepareWhoisUrl()
{
    qDebug("[Dialog::prepareWhoisUrl]");

    if (m_ui->serverIpLineEdit->text().isEmpty()) {
        return QString();
    }

    QString url("http://");
    url += m_ui->serverIpLineEdit->text();
    url += ":" + m_ui->portLineEdit->text();
    url += gametype();

    // add params
    url += "?";
    bool firstElement = true;

    if (!m_ui->playerNickLineEdit->text().isEmpty()) {
        if (firstElement) {
            firstElement = false;
        } else {
            url += "&";
        }

        url += "nick=" + m_ui->playerNickLineEdit->text();
    }

    qDebug() << "[Dialog::prepareWhoisUrl] url is: " << url;

    return url;
}


void Dialog::sendPacketToServer()
{
    qDebug("[Dialog::sendPacketToServer]");

    bool ok;
    int port = m_ui->portLineEdit->text().toInt(&ok);

    if (!ok) {
        showErrorMsg("Invalid port");
        return;
    }

    if (m_ui->serverIpLineEdit->text().isEmpty()) {
        showErrorMsg("Ip is empty");
        return;
    }

//     if (m_socket->peerAddress().isNull()) {
//         qDebug("eh?");
//         return;
//     }

    // TODO trasform in HTTP GET/POST/PUT requests
    QString req;
    int comboIndex = m_ui->commandCombo->currentIndex();

    if (comboIndex == 0) {                                      // ADD
        qDebug("[Dialog::sendPacketToServer] ADD");
    } else if (comboIndex == 1) {                               // BAN
        qDebug("[Dialog::sendPacketToServer] BAN");
    } else if (comboIndex == 2) {                               // IS_BANNED
        qDebug("[Dialog::sendPacketToServer] IS BANNED");
    } else if (comboIndex == 3) {                               // WHOIS
        qDebug("[Dialog::sendPacketToServer] WHOIS");
        req += prepareWhoisUrl();
    } else {
        return;
    }

    m_reply = m_netManager->get(QNetworkRequest(QUrl((req))));

    connect(m_reply, SIGNAL(finished()), this, SLOT(onReplyReadyRead()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));

//     m_socket->writeDatagram(prepareMessage(), QHostAddress(m_ui->serverIpLineEdit->text()), port);
}

void Dialog::setupSignalsAndSlots()
{
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_ui->previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtonClicked()));
    connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));
    connect(m_ui->serverIpLineEdit, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
}

void Dialog::showErrorMsg(const QString& errMsg)
{
    // create error message and show Error Tab
    QString errorMsg;
    errorMsg.append(QTime::currentTime().toString("<b>Error@HH:mm</b>"));
    errorMsg.append(" - ");
    errorMsg.append(errMsg);

    // add error message to error tab and change focus
    m_ui->errorTextEdit->append(errorMsg);
    m_ui->tabWidget->setCurrentIndex(1);
}
