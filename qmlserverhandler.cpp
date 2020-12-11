#include "qmlserverhandler.h"

qmlServerHandler::qmlServerHandler(QObject *parent) : QObject(parent)
{
    sserver = nullptr;
}

void qmlServerHandler::startServer()
{
    qDebug() << Q_FUNC_INFO << endl;
    if (sserver)
    {
        sserver->stopServer();
    }
    else
    {
        sserver = new StreamServer();
    }
    connect(sserver, SIGNAL(messageReady(QString)), this, SIGNAL(messageReady(QString)));
    sserver->startServer();
}

void qmlServerHandler::stopServer()
{
    qDebug() << Q_FUNC_INFO << endl;
    if (sserver)
        sserver->stopServer();
    disconnect(sserver, SIGNAL(messageReady(QString)), this, SIGNAL(messageReady(QString)));
}

void qmlServerHandler::toggleServerState()
{
    if (sserver)
    {
        if (sserver->isRunning())
            stopServer();
        else
            startServer();
    }
    else
    {
        startServer();
    }
}

bool qmlServerHandler::getServerState()
{
    if(sserver)
        return sserver->isRunning();
    else
        return false;
}
