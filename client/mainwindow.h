#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include "gametable.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
    GameTable *gameTable;

    QString ip;
    int port;

    void openGameTable();

signals:
    void sendServerInfo(QString ip, int port);

private slots:
    void joinDialog();
    void receiveIpPort(QString ip, int port);

};

#endif // MAINWINDOW_H
