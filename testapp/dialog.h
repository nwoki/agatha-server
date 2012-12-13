/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011-2012 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */

#include <QtGui/QWidget>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QUdpSocket>

namespace Ui {
    class Main;
}

class QAbstractButton;
class QNetworkAccessManager;
class QProcess;

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
    void onReadyRead();
    void onPreviewButtonClicked();
    void onSocketError(QAbstractSocket::SocketError err);

    // net reply slots
    void onReplyError(QNetworkReply::NetworkError error);
    void onReplyReadyRead();

private:
    QVariantMap createPlayerMap();                          /** create player map to send to server */

    QString gametype() const;

    /** prepare different message types */
    QByteArray prepareMessage();                            /** prepare json message to send to server */

    QString prepareWhoisUrl();                               /** prepares the GET url for a WHOIS request */

    void sendPacketToServer();
    void setupSignalsAndSlots();
    void showErrorMsg(const QString &errMsg);

    Ui::Main *m_ui;
    QUdpSocket *m_socket;
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_netManager;
};
