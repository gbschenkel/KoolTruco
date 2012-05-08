/*
 * player.h
 *
 *  Created on: Jun 23, 2009
 *      Author: schenkel
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <QtCore/QString>

#include "card.h"
#include "thread.h"

class Player : public QObject
{
    Q_OBJECT

public:
    Player();
    virtual ~Player();

    void setNickname(QString n);
    void setCards(int i, Card *receivedCard);
    void setCards(Card *c1, Card *c2, Card *c3);
    void setConnectionThread(Thread *thread);

    bool isConnected();
    void setPlayedCardValue(int value);
    int getPlayedCard();
    void getThread();
    void sendMessage(QString data);
    void setHand(bool theHand);
    bool isHand();
    void yourTurn();
    bool hasPlayed();
    void setPlayed(bool played);

    QString nickname();
    Card* card(int i);

signals:
    void toClient(QString message);
    void fromPlayer(QString message);

private:
    int cardValue;
    Card *cards[3];
    QString nick;
    Thread *thread;
    bool connected;
    bool played;
    bool hand;

    void setConnected(bool boolean);

};

#endif /* PLAYER_H_ */
