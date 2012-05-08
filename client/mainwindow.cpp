
#include <QDesktopWidget>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDesktopWidget *widget = QApplication::desktop();
    QRect dim = widget->screenGeometry();
    setGeometry((dim.width() - width()) / 2, (dim.height() - height()) / 2, width(), height());

    connect(ui->joinButton, SIGNAL(clicked()), this, SLOT(joinDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// This part i implement the slots
// This SLOT call the join dialog where it get the ip and port.
void MainWindow::joinDialog()
{
    dialog = new Dialog;
    connect(dialog, SIGNAL(sendServerInfo(QString,int)), this, SLOT(receiveIpPort(QString,int)));
    connect(dialog, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    if (dialog->exec())
        emit openGameTable();
}

// This SLOT receive the ip and port from dialog window.
void MainWindow::receiveIpPort(QString ip, int port)
{
    this->ip = ip;
    this->port = port;
}


void MainWindow::openGameTable()
{
    gameTable = new GameTable;
    connect(gameTable, SIGNAL(closing()), this, SLOT(show()));
    connect(gameTable, SIGNAL(isVisible()), this, SLOT(hide()));
    connect(this, SIGNAL(sendServerInfo(QString,int)), gameTable, SLOT(setIpPort(QString,int)));
    emit sendServerInfo(ip, port);
}
