#ifndef NETWORK_HPP
#define NETWORK_HPP
#include "defines.hpp"
#include <WiFi.h>
#include <WiFiUdp.h>

class Network
{
public:
    Network();
    virtual ~Network();

    bool setupWifi();
    void loopUDP();

private:
    const char *host;
    int port;
    unsigned long previousMillis;
    long interval; // interval to wait for Wi-Fi connection (milliseconds)
};
extern Network network;
#endif // Network_HPP