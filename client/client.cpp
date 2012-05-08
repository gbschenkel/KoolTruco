#include <QMessageBox>
#include <QAbstractSocket>

#include "client.h"

Client::Client(QString address, int port, QObject *parent)
    : QObject(parent), address(address), port(port)
{
    qDebug() << "Rodando" << address << port;
}

void Client::connectServer()
{
    serverSocket = new QTcpSocket;
    connect(serverSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    serverSocket->connectToHost(address, port);

    if(!serverSocket->waitForConnected()) {
        emit error(serverSocket->error());
        return;
    } else {
        thread = new ClientThread(serverSocket);

        connect(thread, SIGNAL(finished()),
                thread, SLOT(deleteLater()));
        connect(thread, SIGNAL(started()),
                thread, SLOT(threadStarted()));
        connect(thread, SIGNAL(fromServer(QString)),
                this, SLOT(fromServer(QString)));
        connect(this, SIGNAL(toServer(QString)),
                thread, SLOT(toServer(QString)));

        thread->start();
        emit isConnected();
    }
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    {
        switch(socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(NULL, tr("Qtruco Client"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(NULL, tr("Qtruco Client"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(NULL, tr("Qtruco Client"),
                                     tr("The following error occurred: %1.")
                                     .arg(serverSocket->errorString()));
        }
    }
}

void Client::fromServer(QString data)
{
    qDebug() << "Client::receiveMessage >>" << data;
    emit receivedMessage(data);
}

void Client::sendMessage(QString text)
{
    qDebug() << "Client::sendMessage: " << text;
    emit toServer(text);
}
