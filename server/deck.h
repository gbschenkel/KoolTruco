/*
 * deck.h
 *
 *  Created on: Oct 22, 2008
 *      Author: schenkel
 */

#ifndef DECK_H_
#define DECK_H_

#include "card.h"

class Deck {
public:
	Deck();
	virtual ~Deck();

        void suffleDeck();
	Card* newCard();
	QString picture();

private:
	Card *deck[40];
	int length;
	int cardTaken[];

	void buildDeck();

};

#endif /* DECK_H_ */
