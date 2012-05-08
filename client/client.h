/*
 * client.h
 *
 *  Created on: Nov 5, 2009
 *      Author: schenkel
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "clientthread.h"
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QString address, int port, QObject *parent = 0);
    void connectServer();

signals:
    void isConnected();
    void toServer(QString data);
    void error(QAbstractSocket::SocketError socketError);
    void receivedMessage(QString data);

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void fromServer(QString data);
    void sendMessage(QString message);

private:
    const QString address;
    const int port;
    QTcpSocket *serverSocket;
    ClientThread *thread;

    //void sendMessage(QString text);
};


#endif /* CLIENT_H_ */
