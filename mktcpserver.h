#ifndef MKTCPSERVER_H
#define MKTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

class mkSocketServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit mkSocketServer(QObject *parent = nullptr);
    void startServer(int port=8888);

public slots:


    // QTcpServer interface
protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};



#endif // MKTCPSERVER_H
