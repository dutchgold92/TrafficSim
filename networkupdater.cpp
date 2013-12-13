#include "networkupdater.h"

NetworkUpdater::NetworkUpdater(QObject *parent, Network *network) :
    QThread(parent)
{
    this->update_interval = DEFAULT_UPDATE_INTERVAL;
    this->network = network;
    this->stopped = false;
    connect(this, SIGNAL(network_updated()), parent, SLOT(draw_network()), Qt::QueuedConnection);
    connect(this, SIGNAL(network_updated()), parent, SLOT(plot()), Qt::QueuedConnection);
}

void NetworkUpdater::run()
{
    for(;;)
    {
        if(this->stopped)
            break;

        this->network->step();
        emit(network_updated());
        this->usleep(this->update_interval * 1000000);
    }

    this->stopped = false;
}

void NetworkUpdater::set_update_interval(float update_interval)
{
    this->update_interval = update_interval;
}

void NetworkUpdater::stop()
{
    this->stopped = true;
}
