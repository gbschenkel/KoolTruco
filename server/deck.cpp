/*
 * deck.cpp
 *
 *  Created on: Oct 22, 2008
 *      Author: schenkel
 */

#include <QTime>
#include "deck.h"

Deck::Deck()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    buildDeck();
    suffleDeck();
}

Deck::~Deck()
{
	// TODO Auto-generated destructor stub
}

void Deck::buildDeck()
{
        //Cartas Mestes = Master/Big Cards
        deck[0] = new Card("sword", 1, 14);
        deck[1] = new Card("club", 1, 13);
        deck[2] = new Card("sword", 7, 12);
        deck[3] = new Card("coin", 7, 11);

        //Cartas Normais = Normal Cards
        deck[4] = new Card("sword", 3, 10);
        deck[5] = new Card("sword", 2, 9);
        deck[6] = new Card("sword", 12, 7);
        deck[7] = new Card("sword", 11, 6);
        deck[8] = new Card("sword", 10, 5);
        deck[9] = new Card("sword", 6, 3);
        deck[10] = new Card("sword", 5, 2);
        deck[11] = new Card("sword", 4, 1);
        deck[12] = new Card("club", 3, 10);
        deck[13] = new Card("club", 2, 9);
        deck[14] = new Card("club", 12, 7);
        deck[15] = new Card("club", 11, 6);
        deck[16] = new Card("club", 10, 5);
        deck[17] = new Card("club", 7, 4);
        deck[18] = new Card("club", 6, 3);
        deck[19] = new Card("club", 5, 2);
        deck[20] = new Card("club", 4, 1);
        deck[21] = new Card("coin", 3, 10);
        deck[22] = new Card("coin", 2, 9);
        deck[23] = new Card("coin", 1, 8);
        deck[24] = new Card("coin", 12, 7);
        deck[25] = new Card("coin", 11, 6);
        deck[26] = new Card("coin", 10, 5);
        deck[27] = new Card("coin", 6, 3);
        deck[28] = new Card("coin", 5, 2);
        deck[29] = new Card("coin", 4, 1);
        deck[30] = new Card("cup", 3, 10);
        deck[31] = new Card("cup", 2, 9);
        deck[32] = new Card("cup", 1, 8);
        deck[33] = new Card("cup", 12, 7);
        deck[34] = new Card("cup", 11, 6);
        deck[35] = new Card("cup", 10, 5);
        deck[36] = new Card("cup", 7, 4);
        deck[37] = new Card("cup", 6, 3);
        deck[38] = new Card("cup", 5, 2);
        deck[39] = new Card("cup", 4, 1);
}

/*
 * @description return a new card from the deck
 * @return card
 */
Card* Deck::newCard()
{
        int card = -1;
        bool equal;
	do{
		card = (qrand() % 40);
		equal = false;
                for (int i=0; i < length; i++)
			if (card == cardTaken[i]){
				equal = true;
				break;
			}
	}while(equal);
	cardTaken[length] = card;
	length++;

	Card* c = deck[card];
	return c;
}

void Deck::suffleDeck()
{
	length = 0;
}
