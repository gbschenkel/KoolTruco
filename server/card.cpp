/*
 * card.cpp
 *
 *  Created on: Oct 22, 2008
 *      Author: schenkel
 */

#include "card.h"

Card::Card()
{

}

Card::Card(QString suit, int number, int value) : cardSuit(suit), cardNumber(number), cardValue(value)
{
}

Card::~Card(){}

QString Card::suit()
{
    return cardSuit;
}

QString Card::number()
{
    QString num;
    num.setNum(cardNumber);
    return num;
}

int Card::value()
{
    return cardValue;
}
