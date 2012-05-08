#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <QThread>
#include <QTcpSocket>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(int socketDescriptor, QObject *parent);

signals:
    void error(QTcpSocket::SocketError socketError);
    void fromClient(QString data);
    void threadRead();

public slots:
    void toClient(QString data);

private slots:
    void socketDisconnected();
    void receiveMessage();
    void threadStarted();

private:
    int socketDescriptor;
    QTcpSocket tcpSocket;
    quint16 blockSize;
    int position;

    QByteArray createQByteArray(QString str);
};

#endif
