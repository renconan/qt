#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H

#include <QThread>
#include <QTimer>
#include "Defpam.h"
#include "Config.h"


class Importthread : public QThread
{
    Q_OBJECT
public:
    explicit Importthread(QObject *parent = 0);
    void setcmd(char *cmd);
    char cmdstr[512];
    void run();

signals:

public slots:

};

#endif // IMPORTTHREAD_H
