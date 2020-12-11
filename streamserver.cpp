#include "streamserver.h"
StreamServer::StreamServer(QObject *parent) : QObject(parent)
{
    server = nullptr;
    m_clients.clear();
    m_isRuning = false;
}

void StreamServer::startServer()
{
    qDebug() << Q_FUNC_INFO;

    if (server && server->isListening())
    {
        stopServer();
    }
    server = new QWebSocketServer("StreamServer", QWebSocketServer::SslMode::NonSecureMode);
    emit messageReady("Starting Server...");

    if (server->listen(QHostAddress::Any, 5555))
    {
        emit messageReady("Server listening at: " + getSelfIP() + ":5555");
        connect(server, &QWebSocketServer::newConnection, this, &StreamServer::onNewConnection);
        connect(server, &QWebSocketServer::serverError, this, &StreamServer::onErrorOrConnectionLost);
    }
    else {
        emit messageReady("Error: " + server->errorString());
        if (server)
            server->deleteLater();
    }
    m_isRuning = true;
}

void StreamServer::stopServer()
{
    emit messageReady("Stopping Server...");
    Q_FOREACH(auto pSocket, m_clients)
    {
        pSocket->close(QWebSocketProtocol::CloseCodeGoingAway, "Server shutting down");
    }
    qDeleteAll(m_clients);
    if (server)
        server->close();
    server->deleteLater();
    server = nullptr;
    m_isRuning = false;
    emit messageReady("Server stopped");
}

void StreamServer::onNewConnection()
{
    qDebug() << Q_FUNC_INFO << "recieved new connection";
    QWebSocket* pSocket = server->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived,
            [this]( QString message){
        auto data = QByteArray::fromStdString(message.toStdString());
        emit messageReady(data);});

    connect(pSocket, &QWebSocket::binaryMessageReceived, [this] (QByteArray message){
        emit messageReady(message);
    });

    connect(pSocket, &QWebSocket::disconnected, this, &StreamServer::onClientDisconnect);

    m_clients << pSocket;
    emit messageReady("Client Connected. Client Address" + pSocket->peerAddress().toString());
}

void StreamServer::onErrorOrConnectionLost(QWebSocketProtocol::CloseCode code)
{
    qDebug() << Q_FUNC_INFO << code;

    emit messageReady("Error");
}

void StreamServer::onClientDisconnect()
{
    qDebug() << Q_FUNC_INFO;
    auto pClient = qobject_cast<QWebSocket*>(sender());
    qDebug() << "socket disconnected" << pClient;
    emit messageReady("SocketDisconnected" + pClient->peerAddress().toString());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

bool StreamServer::isRunning()
{
    return m_isRuning;
}

QString StreamServer::getSelfIP()
{
    qDebug() << __FUNCTION__;

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            QString addressString = address.toString();
            if (addressString.startsWith("192."))
                return addressString;
        }
    }
    return "*********";

}
