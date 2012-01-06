/*
 * srvAgatha
 *
 * This file is part of srvAgatha
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
 *
 */
#include <QtGui/QWidget>

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

private:
    void setupSignalsAndSlots();

    Ui::Main *m_ui;
};
