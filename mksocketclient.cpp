#include "mksocketclient.h"
#include <string>
#include <iostream>
mkSocketClient::mkSocketClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &mkSocketClient::connected);
    connect(socket,  &QTcpSocket::disconnected, this, &mkSocketClient::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &mkSocketClient::readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &mkSocketClient::bytesWritten);

    std::cout << "\033[37;40m------------- Connecting to server ----------------\033[0m\n"<<std::flush;


}

void mkSocketClient::connect2Server(QString address)
{
    socket->connectToHost(address, 8888);
    qDebug() << "connect2Server--: " << address;
    serverAddress = address;
    if(!socket->waitForConnected(2000))
    {
        qDebug() << "Error--: " << socket->errorString();
    }
}

void mkSocketClient::write(QString str)
{
    socket->write(str.toStdString().c_str());
}


void mkSocketClient::connected()
{

     std::cout << "\033[37;40m\t!!! <Connected to server:"<<serverAddress.toStdString().c_str()<<"> !!!\033[0m\n\n"<<std::flush;
    bSocketOpen=true;
   // socket->write("Hello mkServer from client;");
     emit connectedSignal();
}

void mkSocketClient::disconnected()
{
    bSocketOpen=false;
    //qDebug() << "Disconnected!";
}

void mkSocketClient::bytesWritten(qint64 bytes)
{
    //qDebug() << "We wrote: " << bytes;
}

void mkSocketClient::readyRead()
{
    arrBuffer.append(socket->readAll());
    //qDebug() << "Reading... from socket serer"<<arrBuffer;//<<data;
    while(1)
    {
        size_t n = arrBuffer.toStdString().find('\n');
        if (n != std::string::npos)// found one command...
        {
            ringBuffer.write(arrBuffer.data(), n);
           // ringBuffer.print();
            arrBuffer.remove(0,n+1);
        }
        else break;
    }
}
