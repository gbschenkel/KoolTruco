/*
 * clientThread.h
 *
 *  Created on: Dec 3, 2009
 *      Author: schenkel
 */

#ifndef CLIENTTHREAD_H_
#define CLIENTTHREAD_H_

#include <QThread>
#include <QTcpSocket>

class ClientThread : public QThread
{
    Q_OBJECT

public:
    ClientThread(QTcpSocket *socket, QObject *parent = 0);
    ~ClientThread();

    void connectServer(const QString &hostName, quint16 port);
    void run();
    void sendMessage(int code, QString text);

signals:
    void error(QAbstractSocket::SocketError socketError);
    void fromServer(QString data);

public slots:
    void toServer(QString data);

private slots:
    void socketDisconnected();
    void receiveMessage();
    void threadStarted();


private:
    int socketDescriptor;
    QTcpSocket *server;
    quint16 blockSize;
    QByteArray createQByteArray(QString str);

};

#endif /* CLIENTTHREAD_H_ */
