#ifndef NETWORKUPDATER_H
#define NETWORKUPDATER_H

#define DEFAULT_UPDATE_INTERVAL 1.5

#include <QThread>
#include <network.h>

class NetworkUpdater : public QThread
{
    Q_OBJECT
private:
    double update_interval;
    Network *network;
    bool stopped;
    void run();
    void run_once();
public:
    explicit NetworkUpdater(QObject *parent = 0, Network *network = 0);
    void set_update_interval(float update_interval);
    void stop();
signals:
    void network_updated();
};

#endif // NETWORKUPDATER_H
