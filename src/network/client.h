#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkInterface>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(QString ip, int port); //depois mudar para o codigo
    void connectC1(int serverPort);
    void connectC2(QString joincode);
    void sendData(const QByteArray &data);
    QString readJoincode();
    QString socket_readyRead();
    QString gameSocketRead();
    int getID(){
        return myID;
    }

signals:
    void dataReady(QString);
    void readyRead();

private slots:



private:
    int myID;
    QTcpSocket _tcpSocket; // create tcpsocket in client
    QString _ip; //ip that will be changed for a join code
    int _port; // port that will be changed for a j

};

#endif // CLIENT_H
