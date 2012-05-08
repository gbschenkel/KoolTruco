#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <QtGui/QMainWindow>
#include <QCloseEvent>

class Client;

namespace Ui {
    class GameTable;
}

class GameTable : public QMainWindow
{
    Q_OBJECT

public:
    GameTable(QWidget *parent = 0);
    ~GameTable();
    void closeEvent(QCloseEvent *event);

private:
    int numberOfPlayers;
    QString deck;
    QString type;
    Ui::GameTable *ui;
    QString ip;
    int port;
    Client *client;
    void createClient();
    void sendPlayedCard(QString message);
    void showMyPlayedCard(QPixmap pixmap);
    void setDefault();
    // The functions below if for handle the communication
    // with the server.
    void (GameTable::*method[4])(QString);
    void setPlayerName(QString playerName);
    void setCards(QString playerCard);
    void setPlayedCard(QString message);
    void yourTurn(QString message);
    void nextMatch(QString message);

signals:
    void closing();
    void isVisible();
    void sendMessage(QString message);

private slots:
    void setIpPort(QString ip, int port);
    void connected();
    void readMessage(QString message);
    void on_playerButton_card0_clicked();
    void on_playerButton_card1_clicked();
    void on_playerButton_card2_clicked();
};

#endif // GAMETABLE_H
