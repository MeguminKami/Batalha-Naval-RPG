#include "client.h"

#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    //_tcpSocket = new QTcpSocket(this);
    connect(&_tcpSocket, &QTcpSocket::readyRead, this, &Client::socket_readyRead);
}

// Connect to server function
void Client::connectToServer(QString ip, int port) {
    if(_tcpSocket.isOpen())
    {
        if(ip == _ip && port == _port)
        {
            return;
        }
        _tcpSocket.close();
        _ip = ip;
        _port = port;
        _tcpSocket.connectToHost(_ip, _port);
    }
    qDebug() << "Connected to the server!";
}

// Connects client 1 to the server
void Client::connectC1(int serverPort)
{
    QString ip = "127.0.0.1";

    if(_tcpSocket.isOpen())
    {
        if(ip == _ip && serverPort == _port)
        {
            return;
        }
        _tcpSocket.close();
    }
    _ip = ip;
    _port = serverPort;

    _tcpSocket.connectToHost(_ip, _port);

    qDebug() << "Client1 - Connected to the server!";

    myID = 1;
}

// Reads data
QString Client::socket_readyRead()
{
    QByteArray data;
    while (_tcpSocket.bytesAvailable()) {
        data.append(_tcpSocket.read(1)); // read one byte at a time
    }
    QString dataString = QString::fromUtf8(data); // convert QByteArray to QString
    emit dataReady(dataString); // emit signal with QString
    return dataString;
}

QString Client::gameSocketRead()
{
    QByteArray data;
    while (_tcpSocket.bytesAvailable()) {
        data.append(_tcpSocket.read(1)); // read one byte at a time
    }
    QString dataString = QString::fromUtf8(data); // convert QByteArray to QString
    return dataString;
}

// Connects client 2 to the server
void Client::connectC2(QString joinCode) {
    // Extract the IP address and port from the join code

    // Extract the port (last 4 characters of join code)
    QString portHex = joinCode.right(4);
    bool ok;
    int port = portHex.toInt(&ok, 16);
    if (!ok) {
        qDebug() << "Error parsing port from join code";
        return;
    }

    // Extract the IP address (remaining characters)
    QString ipHex = joinCode.left(joinCode.length() - 4);
    // Convert each pair of hexadecimal characters back to decimal
    QString ip;
    for (int i = 0; i < ipHex.length(); i += 2) {
        QString octetHex = ipHex.mid(i, 2);
        bool ok;
        int octet = octetHex.toInt(&ok, 16);
        if (!ok) {
            qDebug() << "Error parsing IP address from join code";
            return;
        }
        ip += QString::number(octet);
        if (i < ipHex.length() - 2) {
            ip += '.';
        }
    }

    // Now 'ip' contains the IP address and 'port' contains the port
    qDebug() << "Decoded IP: " << ip << " Port: " << port;

    // Continue with the connection logic using the decoded IP and port
    if (_tcpSocket.isOpen()) {
        if (ip == _ip && port == _port) {
            return;
        }
        // _tcpSocket.close();
    }

    _ip = ip;
    _port = port;
    _tcpSocket.connectToHost(_ip, _port);
    qDebug() << "Client 2 Connected to the server!";
    myID = 2;
}

// Sends data
void Client::sendData(const QByteArray &data) {
    _tcpSocket.write(data);
}

QString Client::readJoincode()
{
    return _ip + QStringLiteral(":") + QString::number(_port);
}
