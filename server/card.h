/*
 * card.h
 *
 *  Created on: Oct 22, 2008
 *      Author: schenkel
 */

#ifndef CARD_H_
#define CARD_H_

#include <QtCore/QString>

class Card {
public:
	Card();
	Card(QString suit, int number, int value);
	virtual ~Card();
	QString suit();
        QString number();
	int value();

private:
	QString cardSuit;
	int cardNumber;
	int cardValue;
};

#endif /* CARD_H_ */
