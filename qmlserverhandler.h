#ifndef QMLSERVERHANDLER_H
#define QMLSERVERHANDLER_H

#include <QObject>
#include "streamserver.h"

class qmlServerHandler : public QObject
{
    Q_OBJECT

public:
    explicit qmlServerHandler(QObject *parent = nullptr);
    void startServer();
    void stopServer();
Q_INVOKABLE void toggleServerState();
Q_INVOKABLE bool getServerState();
signals:
    void messageReady(QString data);
public slots:
private:
    StreamServer* sserver;
};

#endif // QMLSERVERHANDLER_H
