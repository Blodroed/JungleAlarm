/**
 * @file NetworkHandler.h
 * @author Marcus Hagen
 */

#ifndef JUNGLEALARM_NETWORK_HANDLER_H
#define JUNGLEALARM_NETWORK_HANDLER_H

#include "mbed.h"

class NetworkHandler {
public:
    NetworkHandler();
    ~NetworkHandler();

    bool connect();

    NetworkInterface* getNetwork() const;

private:
    NetworkInterface* network;
};

#endif // JUNGLEALARM_NETWORK_HANDLER_H