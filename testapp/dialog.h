/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include <QtGui/QWidget>

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
    void onRemoteServerToggled();                       /** called when user changes remote server to connect to */

private:
    QByteArray createPlayerJson();                      /** create player json to send to server */
    QByteArray prepareMessage();                        /** prepare json message to send to server */
    void sendPacketToServer();
    void setupSignalsAndSlots();

    Ui::Main *m_ui;
    QUdpSocket *m_socket;
};
