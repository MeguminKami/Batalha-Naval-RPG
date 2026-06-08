#include "server.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QStringList>

Server::Server(QObject *parent)
    : QObject(parent)
{
    connect(&tcpServer, &QTcpServer::newConnection, this, &Server::acceptClient);
}

QString Server::startServer()
{
    if (!tcpServer.isListening()) {
        tcpServer.listen(QHostAddress::AnyIPv4, 0);
    }

    return makeJoinCode();
}

int Server::whatPort() const
{
    return static_cast<int>(tcpServer.serverPort());
}

void Server::acceptClient()
{
    while (tcpServer.hasPendingConnections()) {
        QTcpSocket *socket = tcpServer.nextPendingConnection();

        connect(socket, &QTcpSocket::disconnected, this, &Server::handleDisconnect);

        if (!client1) {
            client1 = socket;
            connect(client1, &QTcpSocket::readyRead, this, &Server::relayClient1Data);
            emit Client1Connected();
            continue;
        }

        if (!client2) {
            client2 = socket;
            connect(client2, &QTcpSocket::readyRead, this, &Server::relayClient2Data);
            emit Client2Connected();
            continue;
        }

        socket->disconnectFromHost();
        socket->deleteLater();
    }
}

void Server::relayClient1Data()
{
    relay(client1, client2);
}

void Server::relayClient2Data()
{
    relay(client2, client1);
}

void Server::handleDisconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == client1) {
        client1 = nullptr;
    } else if (socket == client2) {
        client2 = nullptr;
    }

    if (socket) {
        socket->deleteLater();
    }

    emit clientDisconnect();
}

QString Server::makeJoinCode() const
{
    QString code;
    const QStringList octets = localIPv4Address().split('.');
    for (const QString &octet : octets) {
        code.append(QString("%1").arg(octet.toInt(), 2, 16, QChar('0')).toUpper());
    }

    code.append(QString("%1").arg(whatPort(), 4, 16, QChar('0')).toUpper());
    return code;
}

QString Server::localIPv4Address() const
{
    const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses) {
        if (address.protocol() != QAbstractSocket::IPv4Protocol) {
            continue;
        }

        if (!address.isLoopback()) {
            return address.toString();
        }
    }

    return QStringLiteral("127.0.0.1");
}

void Server::relay(QTcpSocket *from, QTcpSocket *to)
{
    if (!from) {
        return;
    }

    const QByteArray data = from->readAll();
    if (!to || data.isEmpty()) {
        return;
    }

    to->write(data);
    to->flush();
}
