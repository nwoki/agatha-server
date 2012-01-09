/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "dialog.h"
#include "ui_dialog.h"

#include <qjson/serializer.h>

#include <QtCore/QVariantList>
#include <QtGui/QAbstractButton>
#include <QtNetwork/QUdpSocket>

Dialog::Dialog(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::Main)
    , m_socket(new QUdpSocket)
{
    m_ui->setupUi(this);

    // set deefault values
    m_ui->localhostRadioButton->setChecked(true);
    m_ui->portLineEdit->setText("1337");
    m_ui->playerGearLineEdit->setText("GLAORAA");
    m_ui->playerIpLineEdit->setText("90.54.12.78");
    m_ui->playerNickLineEdit->setText("[LPG]KiicK-aSS");
    m_ui->playerWeaponModeLineEdit->setText("00000111220000020002");

    QStringList commands;
    commands << "add" << "delete" << "ban" << "isBanned" << "unban";
    m_ui->commandCombo->addItems(commands);

    setupSignalsAndSlots();
    show();
}

Dialog::~Dialog()
{
    delete m_ui;
}

QByteArray Dialog::createPlayerJson()
{
    QVariantMap player;

    player.insert("nick", m_ui->playerNickLineEdit->text());
    player.insert("ip", m_ui->playerIpLineEdit->text());
    player.insert("gear", m_ui->playerGearLineEdit->text());
    player.insert("weaponMode", m_ui->playerWeaponModeLineEdit->text());

    QJson::Serializer serializer;
    return serializer.serialize(player);
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

void Dialog::onPreviewButtonClicked()
{
    if (!m_ui->textEdit->toPlainText().isEmpty()) {
        m_ui->textEdit->clear();
    }

    m_ui->textEdit->setText(prepareMessage());
}

/**
 * This function disconnects previous connection when remote server is changed
 * by the user
 */
void Dialog::onRemoteServerToggled()
{
    m_socket->disconnectFromHost();
}


QByteArray Dialog::prepareMessage()
{
    QVariantMap preview;
    preview.insert("game", "URT_411");
    preview.insert("command", m_ui->commandCombo->currentText());
    preview.insert("playerInfo", createPlayerJson());

    QJson::Serializer serializer;
    return serializer.serialize(preview);
}

void Dialog::sendPacketToServer()
{
    bool ok;
    int port = m_ui->portLineEdit->text().toInt(&ok);

    if (!ok) {
        /// TODO error, how to let user know?
        return;
    }

    if (m_ui->localhostRadioButton->isChecked()) {
        if (m_socket->peerAddress() != QHostAddress::LocalHost || m_socket->peerPort() != port) {
            m_socket->connectToHost(QHostAddress::LocalHost, port);
        }
    } else if (m_ui->customAddressRadioButton->isChecked()) {
        if (m_ui->customAddressLineEdit->text().isEmpty()) {
            ///TODO error, let user know?
            return;
        } else if (m_socket->peerAddress() != QHostAddress(m_ui->customAddressLineEdit->text())) {
            m_socket->connectToHost(QHostAddress(m_ui->customAddressLineEdit->text()), port);
        }
    }

    // send packet
    qDebug() << "Writing to host: " << prepareMessage();
    m_socket->write(prepareMessage());
}

void Dialog::setupSignalsAndSlots()
{
    connect(m_ui->localhostRadioButton, SIGNAL(toggled()), this, SLOT(onRemoteServerToggled()));
    connect(m_ui->previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtonClicked()));
    connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));
}
