#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>

#include "deck.h"
#include "player.h"

struct Team{
    int totalPoints;
};

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);

    void setPlayerNick(QString,Player*);
    void playedCard(QString,Player*);

protected:
    void incomingConnection(int socketDescriptor);

signals:
    void toClient(QString data);
    void isFull();

private slots:
    void fromClient(QString data);
    void checkPlayers();
    void startGame();

private:
    Team team0;
    Team team1;
    int numberOfPlayers;
    int playerTurn;
    int hand;
    int round;
    int roundPoints;
    int *cardsOfRoundOne;
    int *cardsOfRoundTwo;
    int *cardsOfRoundThree;
    QStringList fortunes;
    Deck *deck;
    Player *players[4];

    void nextPlayer();
    int getPlayerTurn();
    void setPlayerTurn(int turn);
    void sendCardsToClients();
    void checkGame();
    void setRound(int round);
    int getRound();
    void nextRound();
    void restartGame();

    void (Server::*method[4])(QString,Player*);

};

#endif
