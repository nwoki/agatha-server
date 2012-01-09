/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */
#include <QtGui/QWidget>

/// TODO Icon with player info settings to send to server
/// TODO Add QJson

namespace Ui {
    class Main;
}

class QAbstractButton;
class QUdpSocket;

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private Q_SLOTS:
    void onButtonBoxClicked(QAbstractButton *button);
    void onPreviewButtonClicked();

private:
    void sendPacketToServer();
    void setupSignalsAndSlots();

    Ui::Main *m_ui;
    QUdpSocket *m_socket;
};
