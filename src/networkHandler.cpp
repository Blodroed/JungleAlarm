/**
 * @file NetworkHandler.cpp
 * @author Marcus Hagen
 */

#include "../include/networkHandler.h"
#include <iostream>

NetworkHandler::NetworkHandler() : network(NetworkInterface::get_default_instance()) {}

NetworkHandler::~NetworkHandler() {
}

bool NetworkHandler::connect() {
    if (!network) {
        std::cout << "Network interface is not available.\n";
        return false;
    }
    nsapi_error_t status = network->connect();
    if (status != NSAPI_ERROR_OK) {
        std::cout << "Unable to connect to network, error: " << status << "\n";
        return false;
    }
    std::cout << "Network connected successfully.\n";
    return true;
}

NetworkInterface* NetworkHandler::getNetwork() const {
    return network;
}