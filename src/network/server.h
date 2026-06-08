#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

    QString startServer();
    int whatPort() const;

signals:
    void Client1Connected();
    void Client2Connected();
    void clientDisconnect();

private slots:
    void acceptClient();
    void relayClient1Data();
    void relayClient2Data();
    void handleDisconnect();

private:
    QTcpServer tcpServer;
    QTcpSocket *client1 = nullptr;
    QTcpSocket *client2 = nullptr;

    QString makeJoinCode() const;
    QString localIPv4Address() const;
    void relay(QTcpSocket *from, QTcpSocket *to);
};

#endif // SERVER_H
