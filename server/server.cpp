#include "server.h"
#include "thread.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    connect(this, SIGNAL(isFull()), SLOT(startGame()));
    numberOfPlayers = 2;
    for (int i = 0; i < numberOfPlayers; i++){
        players[i] = new Player();
    }
    playerTurn = 0;
    round = 0;
    hand = 0;
    cardsOfRoundOne = new int[numberOfPlayers];
    cardsOfRoundTwo = new int[numberOfPlayers];
    cardsOfRoundThree = new int[numberOfPlayers];

    method[00] = &Server::setPlayerNick;
    method[01] = &Server::playedCard;
}

void Server::incomingConnection(int socketDescriptor)
{
    Thread *thread = new Thread(socketDescriptor, this);
    for (int i = 0; i < numberOfPlayers; i++){
        if (!players[i]->isConnected()){
            players[i]->setConnectionThread(thread);
            connect(players[i], SIGNAL(fromPlayer(QString)), this, SLOT(fromClient(QString)));
            break;
        }
    }
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), thread, SLOT(threadStarted()));
    connect(thread, SIGNAL(threadRead()), this, SLOT(checkPlayers()));
    thread->start();
}

void Server::fromClient(QString message)
{
    Player* pl = static_cast<Player*>(sender());
    int code = message.mid(0, 2).toInt();
    (this->*method[code])(message.mid(2),pl);
}

void Server::checkPlayers()
{
    int numberOfConnectedPlayers;
    numberOfConnectedPlayers = 0;

    for (int i = 0; i < numberOfPlayers; i++) {
        if (players[i]->isConnected()) {
            numberOfConnectedPlayers++;
        }
    }
    if (numberOfPlayers == numberOfConnectedPlayers) {
        startGame();
    }
}

void Server::startGame()
{
    deck = new Deck();
    for (int card = 0; card < 3; card++) {
        for(int i = 0; i < numberOfPlayers; i++) {
            players[i]->setCards(card, deck->newCard());
        }
    }
    players[getPlayerTurn()]->yourTurn();
}

void Server::playedCard(QString message, Player *pl)
{
    qDebug() << "Server::playedCard";
    QString num;
    QString suit;
    int value;
    if (message == "0") {
        num = pl->card(0)->number();
        suit = pl->card(0)->suit();
        value = pl->card(0)->value();
    } else if (message == "1") {
        num = pl->card(1)->number();
        suit = pl->card(1)->suit();
        value = pl->card(1)->value();
    } else {
        num = pl->card(2)->number();
        suit = pl->card(2)->suit();
        value = pl->card(2)->value();
    }
    QString newMessage;
    QString code;

    code = "04"; //code for played card.
    newMessage = num + suit;
    qDebug() << newMessage;
    if (numberOfPlayers == 4) {
        if (players[0] == pl) {
            players[1]->sendMessage(code + "03" + newMessage);
            players[2]->sendMessage(code + "02" + newMessage);
            players[3]->sendMessage(code + "01" + newMessage);
            players[0]->setPlayed(true);
            players[0]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[0] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[0] = value;
            if (getRound() == 2)
                cardsOfRoundThree[0] = value;
        } else if (players[1] == pl) {
            players[0]->sendMessage(code + "01" + newMessage);
            players[2]->sendMessage(code + "03" + newMessage);
            players[3]->sendMessage(code + "02" + newMessage);
            players[1]->setPlayed(true);
            players[1]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[1] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[1] = value;
            if (getRound() == 2)
                cardsOfRoundThree[1] = value;
        } else if (players[2] == pl) {
            players[0]->sendMessage(code + "02" + newMessage);
            players[1]->sendMessage(code + "01" + newMessage);
            players[3]->sendMessage(code + "03" + newMessage);
            players[2]->setPlayed(true);
            players[2]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[2] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[2] = value;
            if (getRound() == 2)
                cardsOfRoundThree[2] = value;
        } else {
            players[0]->sendMessage(code + "03" + newMessage);
            players[1]->sendMessage(code + "02" + newMessage);
            players[2]->sendMessage(code + "01" + newMessage);
            players[3]->setPlayed(true);
            players[3]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[3] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[3] = value;
            if (getRound() == 2)
                cardsOfRoundThree[3] = value;
        }
        if (players[0]->hasPlayed() && players[1]->hasPlayed() &&
                players[2]->hasPlayed() && players[3]->hasPlayed()) {
            checkGame();
        } else {
            nextPlayer();
        }

    } else {
        if (players[0] == pl) {
            players[1]->sendMessage(code + "02" + newMessage);
            players[1]->setPlayed(true);
            players[1]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[0] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[0] = value;
            if (getRound() == 2)
                cardsOfRoundThree[0] = value;
        } else {
            players[0]->sendMessage(code + "02" + newMessage);
            players[0]->setPlayed(true);
            players[0]->setPlayedCardValue(value);
            if (getRound() == 0)
                cardsOfRoundOne[1] = value;
            if (getRound() == 1)
                cardsOfRoundTwo[1] = value;
            if (getRound() == 2)
                cardsOfRoundThree[1] = value;
        }

        if (players[0]->hasPlayed() && players[1]->hasPlayed()) {
            qDebug() << "All players have played!";
            checkGame();
        } else {
            qDebug() << "Someone not have played yet, going to next player";
            nextPlayer();
        }
    }
}

void Server::setPlayerNick(QString nick, Player *pl)
{

}

void Server::nextPlayer()
{
    playerTurn++;
    if (playerTurn > numberOfPlayers-1)
        playerTurn = 0;
    players[playerTurn]->yourTurn();
}

void Server::setPlayerTurn(int turn)
{
    playerTurn = turn;
    players[playerTurn]->yourTurn();
}

int Server::getPlayerTurn()
{
    return playerTurn;
}

void Server::nextRound()
{
    round++;
    if (round > 3)
        round = 0;
    for (int i = 0; i < numberOfPlayers; i++)
        players[i]->setPlayed(false);
    qDebug() << "Round: " << round;
}

void Server::setRound(int round)
{
    this->round = round;
}

int Server::getRound()
{
    return round;
}

void Server::checkGame()
{
    qDebug() << "Server::checkGame()";
    int highValueRoundOne;
    int highValueRoundTwo;
    int highValueRoundThree;
    int playerWinnerRoundOne;
    int playerWinnerRoundTwo;
    int playerWinnerRoundThree;
    bool team0Winner;
    bool team1Winner;
    highValueRoundOne = 0;
    highValueRoundTwo = 0;
    highValueRoundThree = 0;
    playerWinnerRoundOne = -1;
    playerWinnerRoundTwo = -1;
    playerWinnerRoundThree = -1;
    team0Winner = false;
    team1Winner = false;

    // round = [0,1,2]
    //qDebug() << "Card of player0:" << cardsOfRoundOne[0];
    //qDebug() << "Card of player1:" << cardsOfRoundOne[1];
    for (int i = 0; i < numberOfPlayers; i++) {
        if (cardsOfRoundOne[i] > highValueRoundOne) {//when round 0
            qDebug() << "Cards round one: " << cardsOfRoundOne[i];
            highValueRoundOne = cardsOfRoundOne[i];
            playerWinnerRoundOne = i;
        }
        if (getRound() != 0) //when round 1
            if (cardsOfRoundTwo[i] > highValueRoundTwo) {
                qDebug() << "Cards round two: " << cardsOfRoundTwo[i];
                highValueRoundTwo = cardsOfRoundTwo[i];
                playerWinnerRoundTwo = i;
            }
        if (getRound() == 2) { //when round 2
            if (cardsOfRoundThree[i] > highValueRoundThree) {
                qDebug() << "Cards round three: " << cardsOfRoundThree[i];
                highValueRoundThree = cardsOfRoundThree[i];
                playerWinnerRoundThree = i;
            }
        }
        qDebug() << "High Value Card of Round[0]" <<highValueRoundOne;
        qDebug() << "High Value Card of Round[1]" <<highValueRoundTwo;
        qDebug() << "High Value Card of Round[2]" <<highValueRoundThree;
    }

    qDebug() << "Winner first round" << playerWinnerRoundOne;
    qDebug() << "Winner second round" << playerWinnerRoundTwo;
    qDebug() << "Winner third round" << playerWinnerRoundThree;

    if (round > 0) {
        qDebug() << "Round 2 tests";
        if (playerWinnerRoundOne % 2 == 0) {
            if (playerWinnerRoundTwo % 2 == 0) {
                team0Winner = true; //ganhou 1 e 2
                qDebug() << "Team0 Win round 1 and 2";
            } else {//perdeu 2
                if (round == 2) {
                    if (playerWinnerRoundThree % 2 == 0) {
                        team0Winner = true; // ganhou 1 e 3
                        qDebug() << "Team0 Win round 1 and 3";
                    } else {
                        team1Winner = true; // perdeu 2 e 3
                        qDebug() << "Lost round 2 and 3";
                    }
                }
            }
        } else {//perdeu 1
            if (playerWinnerRoundTwo %2 == 0) {
                if (round == 2) {
                    if (playerWinnerRoundThree % 2 == 0) {
                        team0Winner = true; // ganhou 2 e 3
                        qDebug() << "Team0 Win round 2 and 3";
                    } else {
                        team1Winner = true; // perdeu 1 e 3
                        qDebug() << "Team0 Lost round 1 and 3";
                    }
                }
            } else {
                team1Winner = true; // perdeu 1 e 2
                qDebug() << "Lost round 1 and 2";
            }
        }
    }

    if (!team0Winner && !team1Winner) {
        // The player who win this round play again
        if (round == 0) {
            qDebug() << "No game's winner | round[0]";
            setPlayerTurn(playerWinnerRoundOne);
        }
        if (round == 1) {
            qDebug() << "No game's winner | round[1]";
            setPlayerTurn(playerWinnerRoundTwo);
        }
    } else { // faz algo pq alguem ganhou
        if (team0Winner) {
            qDebug() << "Team0 win this match!";
            //restartGame();
        }
        if (team1Winner) {
            qDebug() << "Team1 win this match!";
            //restartGame();
        }
    }
    nextRound();
}

// This procedure has issues in some place, need re-check
void Server::restartGame()
{
    QString code;
    QString message;
    code == "02";
    message = code + "nothing";
    for (int i = 0; i < numberOfPlayers; i++){
        players[i]->sendMessage(message);
    }
    int add;
    playerTurn = 0;
    round = 0;
    hand++;
    if (hand > numberOfPlayers-1)
        hand = 0;
    deck->suffleDeck();
    add = 0;
    for (int card = 0; card < 3; card++) {
        for(int i = 0; i < numberOfPlayers; i++) {
            if (hand == 0) {
                players[i]->setCards(card, deck->newCard());
            }
            if (hand == 1) {
                add = i + hand;
                if (add > 3) {
                    players[add-4]->setCards(card, deck->newCard());
                } else {
                    players[add]->setCards(card, deck->newCard());
                }
            }
            if (hand == 2) {
                add = i + hand;
                if (add > 3) {
                    players[add-4]->setCards(card, deck->newCard());
                } else {
                    players[add]->setCards(card, deck->newCard());
                }
            }
            if (hand == 3) {
                add = i + hand;
                if (add > 3) {
                    players[add-4]->setCards(card, deck->newCard());
                } else {
                    players[add]->setCards(card, deck->newCard());
                }
            }
        }
    }
    setPlayerTurn(hand);
    players[getPlayerTurn()]->yourTurn();
}
