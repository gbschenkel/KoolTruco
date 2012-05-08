#include <QDesktopWidget>

#include "gametable.h"
#include "ui_gametable.h"
#include "client.h"

GameTable::GameTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameTable)
{
    ui->setupUi(this);
    QDesktopWidget *widget = QApplication::desktop();
    QRect dim = widget->screenGeometry();
    setGeometry((dim.width() - width()) / 2, (dim.height() - height()) / 2, width(), height());
    numberOfPlayers = 2;
    //deck = ":/deck/pia/";
    deck = ":/deck/spanish/";
    type = ".png";
    setDefault();
    method[00] = &GameTable::setPlayerName;
    method[01] = &GameTable::yourTurn;
    method[02] = &GameTable::nextMatch;
    method[03] = &GameTable::setCards;
    method[04] = &GameTable::setPlayedCard;
}

GameTable::~GameTable()
{

}

void GameTable::setIpPort(QString ip, int port)
{
    this->ip = ip;
    this->port = port;
    createClient();
}

void GameTable::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}

void GameTable::createClient()
{
    client = new Client(ip, port);
    connect(client, SIGNAL(isConnected()), this, SLOT(show()));
    connect(client, SIGNAL(isConnected()), this, SLOT(connected()));
    connect(client, SIGNAL(receivedMessage(QString)), this, SLOT(readMessage(QString)));
    connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
    client->connectServer();
}

void GameTable::connected()
{
    emit isVisible();
}

/*
 * This part handle the comunication received from the server
*/
void GameTable::readMessage(QString message)
{
    qDebug() << "gameTable::receiveMessage >>" << message;
    int code = message.mid(0, 2).toInt();
    qDebug() << "Code = " << code;
    qDebug() << "message = " << message.mid(2);
    (this->*method[code])(message.mid(2));

}

void GameTable::setPlayerName(QString playerName)
{
    if(playerName[0] == QChar('0')) {
        ui->playerGroupBox->setTitle(playerName.mid(1));
    } else if(playerName[0] == QChar('1')) {
        ui->player1GroupBox->setTitle(playerName.mid(1));
    } else if(playerName[0] == QChar('2')) {
        ui->player2GroupBox->setTitle(playerName.mid(1));
    } else if(playerName[0] == QChar('3')) {
        ui->player3GroupBox->setTitle(playerName.mid(1));
    }
}

void GameTable::setCards(QString playerCard)
{
    qDebug() << "setCards started";
    qDebug() << deck + playerCard + type;
    if(ui->playerButton_card0->icon().isNull()) {
        ui->playerButton_card0->setIcon(QIcon(deck + playerCard + type));
    } else if(ui->playerButton_card1->icon().isNull()) {
        ui->playerButton_card1->setIcon(QIcon(deck + playerCard + type));
    } else {
        ui->playerButton_card2->setIcon(QIcon(deck + playerCard + type));
    }
}

void GameTable::showMyPlayedCard(QPixmap pixmap)
{
    if(ui->p0Label_card1a->isHidden()) {
        ui->p0Label_card1a->setPixmap(pixmap);
        ui->p0Label_card1a->show();
    } else {
        if(ui->p0Label_card1b->isHidden()) {
            ui->p0Label_card1b->setPixmap(pixmap);
            ui->p0Label_card1b->show();
        } else {
            ui->p0Label_card1c->setPixmap(pixmap);
            ui->p0Label_card1c->show();
        }
    }
}

void GameTable::on_playerButton_card0_clicked()
{
    QPixmap pixmap(ui->playerButton_card0->icon().pixmap(QSize(80,160)));
    showMyPlayedCard(pixmap);
    ui->playerButton_card0->hide();

    QString code = "01";
    QString card = "0";
    QString message;
    message = code + card;
    emit sendMessage(message);
    ui->playerButton_card0->setEnabled(false);
    ui->playerButton_card1->setEnabled(false);
    ui->playerButton_card2->setEnabled(false);

}

void GameTable::on_playerButton_card1_clicked()
{
    QPixmap pixmap(ui->playerButton_card1->icon().pixmap(QSize(80,160)));
    showMyPlayedCard(pixmap);
    ui->playerButton_card1->hide();

    QString code = "01";
    QString card = "1";
    QString message;
    message = code + card;
    emit sendMessage(message);
    ui->playerButton_card0->setEnabled(false);
    ui->playerButton_card1->setEnabled(false);
    ui->playerButton_card2->setEnabled(false);

}

void GameTable::on_playerButton_card2_clicked()
{
    QPixmap pixmap(ui->playerButton_card2->icon().pixmap(QSize(80,160)));
    showMyPlayedCard(pixmap);
    ui->playerButton_card2->hide();

    QString code = "01";
    QString card = "2";
    QString message;
    message = code + card;
    emit sendMessage(message);
    ui->playerButton_card0->setEnabled(false);
    ui->playerButton_card1->setEnabled(false);
    ui->playerButton_card2->setEnabled(false);

}

void GameTable::setDefault()
{
    ui->player1Label_card0a->hide();
    ui->player1Label_card0b->hide();
    ui->player1Label_card0c->hide();
    ui->player2Label_card0a->hide();
    ui->player2Label_card0b->hide();
    ui->player2Label_card0c->hide();
    ui->player3Label_card0a->hide();
    ui->player3Label_card0b->hide();
    ui->player3Label_card0c->hide();
    // Hide the played card slots
    ui->p0Label_card1a->hide();
    ui->p0Label_card1b->hide();
    ui->p0Label_card1c->hide();
    ui->p2Label_card1a->hide();
    ui->p2Label_card1b->hide();
    ui->p2Label_card1c->hide();
    if (numberOfPlayers == 4) {
        ui->p1Label_card1a->hide();
        ui->p1Label_card1b->hide();
        ui->p1Label_card1c->hide();
        ui->p3Label_card1a->hide();
        ui->p3Label_card1b->hide();
        ui->p3Label_card1c->hide();
    }

    // Show card's back from players
    ui->playerButton_card0->show();
    ui->playerButton_card1->show();
    ui->playerButton_card2->show();
    ui->player2Label_card0a->show();
    ui->player2Label_card0b->show();
    ui->player2Label_card0c->show();
    if (numberOfPlayers == 4) {
        ui->player1Label_card0a->show();
        ui->player1Label_card0b->show();
        ui->player1Label_card0c->show();
        ui->player3Label_card0a->show();
        ui->player3Label_card0b->show();
        ui->player3Label_card0c->show();
    }
    ui->playerButton_card0->setEnabled(false);
    ui->playerButton_card1->setEnabled(false);
    ui->playerButton_card2->setEnabled(false);

}

void GameTable::setPlayedCard(QString message)
{
    int player;
    QString card;
    player = message.mid(0,2).toInt();
    card = message.mid(2);
    qDebug() << "setPlayedCard";
    qDebug() << "Player: " << player;
    qDebug() << "Card: " << card;
    qDebug() << "CardPath: " << deck << card << type;
    QPixmap pixmap(deck + card + type);
    if (numberOfPlayers == 2) {
        if(ui->p2Label_card1a->isHidden()) {
            ui->p2Label_card1a->setPixmap(pixmap);
            ui->p2Label_card1a->show();
        } else {
            if(ui->p2Label_card1b->isHidden()) {
                ui->p2Label_card1b->setPixmap(pixmap);
                ui->p2Label_card1b->show();
            } else {
                ui->p2Label_card1c->setPixmap(pixmap);
                ui->p2Label_card1c->show();
            }
        }
    }
}

void GameTable::yourTurn(QString message)
{
    ui->playerButton_card0->setEnabled(true);
    ui->playerButton_card1->setEnabled(true);
    ui->playerButton_card2->setEnabled(true);
}

void GameTable::nextMatch(QString message)
{
    setDefault();
}
