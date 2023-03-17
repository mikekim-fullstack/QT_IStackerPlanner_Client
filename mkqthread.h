#ifndef MKQTHREAD_H
#define MKQTHREAD_H
#include <QTcpSocket>
#include <QThread>
#include <QDebug>
class mkSocketThread : public QThread
{
    Q_OBJECT
public:
   explicit mkSocketThread(qintptr ID, QObject *parent = 0);
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
signals:
    void error(QTcpSocket::SocketError socketerror);
public slots:
    void readyRead();
    void disconnected();
    // QThread interface
protected:
    virtual void run() override;
};



#endif // MKQTHREAD_H
