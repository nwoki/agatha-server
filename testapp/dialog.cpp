/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include "dialog.h"
#include "ui_dialog.h"

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

    setupSignalsAndSlots();
    show();
}

Dialog::~Dialog()
{
    delete m_ui;
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
    /// TODO create QJSon text and show in the textArea
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
        /// TODO do check for same address a previous here aas well
        if (m_ui->customAddressLineEdit->text().isEmpty()) {
            ///TODO error, let user know?
            return;
        }

        m_socket->connectToHost(QHostAddress(m_ui->customAddressLineEdit->text()), port);
    }

    // send packet
    qDebug() << "Writing to host: " << m_ui->textEdit->toPlainText();
    m_socket->write(m_ui->textEdit->toPlainText().toUtf8());
}

void Dialog::setupSignalsAndSlots()
{
    connect(m_ui->previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtonClicked()));
    connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));
}
