/*
 * player.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: schenkel
 */

#include "player.h"

Player::Player() {
    connected = false;
}

Player::~Player() {
    // TODO Auto-generated destructor stub
}

void Player::setNickname(QString n) {
    nick = n;
}

QString Player::nickname() {
    return nick;
}

void Player::setCards(int i, Card *receivedCard){
    cards[i] = receivedCard;

    QString message;
    QString code;
    QString card;
    code = "03";
    card = receivedCard->number();
    card += receivedCard->suit();
    message = code + card;
    emit toClient(message);
    qDebug() << message;

}

void Player::setCards(Card *c1, Card *c2, Card *c3) {
    cards[0] = c1;
    cards[1] = c2;
    cards[2] = c3;

    QString message;
    QString cod;
    QString player;
    QString card;

    /*
         * toClient (Cod + Player + Message)
         * Cod = 01 is to call setCard procedure on Client
         * Player = 00 for set the cards on own Client.
         * Message is the card number and suit, to set the picture on
         * the card buttons
         */
    cod = "01";
    player = "00";

    card = c1->number();
    card += c1->suit();
    message = cod + player + card;
    emit toClient(message);
    qDebug() << message;

    card = c2->number();
    card += c2->suit();
    message = cod + player + card;
    emit toClient(message);
    qDebug() << message;

    card = c3->number();
    card += c3->suit();
    message = cod + player + card;
    emit toClient(message);
    qDebug() << message;
}

Card* Player::card(int i) {
    return cards[i];
}

void Player::sendMessage(QString data) {
    emit toClient(data);
}

void Player::setConnectionThread(Thread *th) {
    thread = th;
    connect(this, SIGNAL(toClient(QString)), thread, SLOT(toClient(QString)));
    connect(thread, SIGNAL(fromClient(QString)), this, SIGNAL(fromPlayer(QString)));
    qDebug() << "Thread Set!";
    setConnected(true);
}

void Player::setConnected(bool boolean){
    connected = boolean;
}

bool Player::isConnected() {
    return connected;
}

void Player::setPlayedCardValue(int value) {
    cardValue = value;
}

int Player::getPlayedCard() {
    return cardValue;
}

bool Player::hasPlayed() {
    return played;
}

void Player::setPlayed(bool played) {
    this->played = played;
}

void Player::setHand(bool theHand) {
    hand = theHand;
}

bool Player::isHand() {
    return hand;
}

void Player::yourTurn() {
    QString code;
    QString message;
    code = "01";
    message = code + "nothing more";
    emit sendMessage(code);
}

void Player::getThread(){
    qDebug() << thread->currentThreadId();
}
