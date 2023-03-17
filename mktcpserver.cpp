#include "mktcpserver.h"
#include "mkqthread.h"
mkSocketServer::mkSocketServer(QObject *parent) : QTcpServer(parent)
{


    // whenever a user connects, it will emit signal

}

void mkSocketServer::startServer(int port)
{
    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}


void mkSocketServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
        qDebug() << socketDescriptor << " Connecting...";

        // Every new connection will be run in a newly created thread
        mkSocketThread *thread = new mkSocketThread(socketDescriptor, this);

        // connect signal/slot
        // once a thread is not needed, it will be beleted later
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
}
