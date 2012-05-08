#include "thread.h"

#include <QtNetwork>

Thread::Thread(int socketDescriptor, QObject *parent) :
    QThread(parent), socketDescriptor(socketDescriptor)
{
    blockSize = 0;
}


void Thread::socketDisconnected()
{
    quit();
}

void Thread::threadStarted()
{
    qDebug() << "Iniciou o thStarted!";
    if(!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    connect(&tcpSocket, SIGNAL(disconnected()),
            this, SLOT(socketDisconnected()));

    connect(&tcpSocket, SIGNAL(readyRead()),
            this, SLOT(receiveMessage()));

    emit threadRead();
}


void Thread::receiveMessage()
{
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    while(!in.atEnd()) {
        if(blockSize == 0) {
            if(tcpSocket.bytesAvailable() < (int) sizeof(quint16))
                return;

            in >> blockSize;
        }

        if(tcpSocket.bytesAvailable() < blockSize)
            return;

        QString str;
        QByteArray tmp;
        in >> tmp;
        str = QString::fromLatin1(qUncompress(tmp));
        qDebug(QTextCodec::codecForLocale()->name());
        qDebug() << "ServerThread::receiveMessage |str| >>" << str;
        qDebug() << "ServerThread::receiveMessage |tmp| >>" << tmp;

        blockSize = 0;
        emit fromClient(str);
    }
}

QByteArray Thread::createQByteArray(QString str)
{
    qDebug() << "Transformando a QString em QByteArray";
    qDebug() << "mensagem " << str;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out << (quint16) 0;
    out << qCompress(str.toLatin1(), 9);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    qDebug() << "Pronto!";
    return block;
}

void Thread::toClient(QString data)
{
    qDebug() << "Iniciou o envio da mensagem";
    QByteArray block = createQByteArray(data);
    tcpSocket.write(block);
}
