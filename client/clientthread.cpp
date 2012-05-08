/*
 * clientThread.cpp
 *
 *  Created on: Dec 3, 2009
 *      Author: schenkel
 */

#include <QtNetwork>

#include "clientthread.h"

ClientThread::ClientThread(QTcpSocket *socket, QObject *parent)
    : QThread(parent), server(socket)
{
    blockSize = 0;
}

ClientThread::~ClientThread()
{
}

void ClientThread::run()
{
    exec();
}

void ClientThread::threadStarted()
{
    //server = new QTcpSocket(this);

    connect(server, SIGNAL(disconnected()),
            this, SLOT(socketDisconnected()));

    connect(server, SIGNAL(readyRead()),
            this, SLOT(receiveMessage()));

}

void ClientThread::socketDisconnected()
{
    exit(0);
    quit();
}

void ClientThread::receiveMessage()
{
    QDataStream in(server);
    in.setVersion(QDataStream::Qt_4_0);

    while(!in.atEnd()) {
        if(blockSize == 0) {
            if(server->bytesAvailable() < (int) sizeof(quint16))
                return;

            in >> blockSize;
        }

        if(server->bytesAvailable() < blockSize)
            return;

        QString str;
        QByteArray tmp;
        in >> tmp;
        str = QString::fromLatin1(qUncompress(tmp));
        qDebug() << "ClientThread::receiveMessage >>" << str;
        blockSize = 0;
        emit fromServer(str);
    }
}

QByteArray ClientThread::createQByteArray(QString str)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out << (quint16) 0;
    out << qCompress(str.toLatin1(), 9);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    return block;
}

void ClientThread::toServer(QString data)
{
    QByteArray block = createQByteArray(data);
    server->write(block);
}
