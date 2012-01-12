/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include <QtGui/QWidget>
#include <QtNetwork/QUdpSocket>

namespace Ui {
    class Main;
}

class QAbstractButton;

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private Q_SLOTS:
    void onButtonBoxClicked(QAbstractButton *button);
    void onDisconnected() { qDebug("DISCONNECTED");};
    void onEditingFinished();
    void onPreviewButtonClicked();
    void onSocketError(QAbstractSocket::SocketError err);

private:
    QVariantMap createPlayerMap();                          /** create player map to send to server */
    QByteArray prepareMessage();                            /** prepare json message to send to server */
    void sendPacketToServer();
    void setupSignalsAndSlots();
    void showErrorMsg(const QString &errMsg);

    Ui::Main *m_ui;
    QUdpSocket *m_socket;
};
