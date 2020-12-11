#ifndef STREAMSERVER_H
#define STREAMSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostInfo>
#include <QNetworkInterface>

class StreamServer : public QObject
{
    Q_OBJECT
public:
    explicit StreamServer(QObject *parent = nullptr);
    void startServer();
    void stopServer();
    void onNewConnection();
    void onErrorOrConnectionLost(QWebSocketProtocol::CloseCode code);
    void onClientDisconnect();
    bool isRunning();
    QString getSelfIP();
signals:
    void messageReady(QString data);
public slots:
private:
    QWebSocketServer* server;
    QList<QWebSocket*> m_clients;
    bool m_isRuning;
};

#endif // STREAMSERVER_H
