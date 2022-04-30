#include "network.hpp"

WiFiUDP udp;

/**
 * @brief Construct a new Network:: Network object
 *
 */
Network::Network()
{
    host = ""; // IP address of the UDP server
    port = 9000;
    previousMillis = 0;
    interval = 30000; // interval to wait for Wi-Fi connection (milliseconds)
}

/**
 * @brief Destroy the Network:: Network object
 *
 */
Network::~Network()
{
}

/**
 * @brief Setup Wi-Fi connection
 *
 * @return true
 * @return false
 */
bool Network::setupWifi()
{
    log_i("Network::setupUDP()");
    log_i("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);

    IPAddress localIP;
    IPAddress gateway;
    IPAddress subnet;

    localIP.fromString(WiFi.localIP().toString());
    gateway.fromString(WiFi.gatewayIP().toString());
    subnet.fromString(WiFi.subnetMask().toString());

    if (!WiFi.config(localIP, gateway, subnet))
    {
        log_e("[INFO]: STA Failed to configure\n");
        return false;
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long currentMillis = millis();
    previousMillis = currentMillis;

    while (WiFi.status() != WL_CONNECTED)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            log_e("Failed to connect to WiFi");
            if (WiFi.status() == WL_CONNECT_FAILED)
            {
                log_e("WiFi connection failed!");
                return false;
            }
            else if (WiFi.status() == WL_CONNECTION_LOST)
            {
                log_e("WiFi connection lost!");
                return false;
            }
            else if (WiFi.status() == WL_DISCONNECTED)
            {
                log_e("WiFi disconnected!");
                return false;
            }
            else if (WiFi.status() == WL_NO_SSID_AVAIL)
            {
                log_e("No SSID available!");
                return false;
            }
            log_i("[INFO]: WiFi connection timed out.\n");
            return false;
            break;
        }
    }

    log_i("WiFi connected!");
    log_i("SSID: %s", WiFi.SSID().c_str());
    log_i("IP address: %s", WiFi.localIP().toString().c_str());
    return true;
}

/**
 * @brief Loop the UDP socket and receive data
 *
 */
void Network::loopUDP()
{
    if (!setupWifi())
    {
        log_i("Wifi failed to connect. Retrying...");
        setupWifi();
    }
    else
    {
        log_i("Wifi connected!");
        log_i("Setting up UDP...");
        uint8_t buffer[1024];              // pass all data into the buffer - make sure it's big enough
        udp.beginPacket(host, port);       // start UDP transmission to host:port
        udp.write(buffer, sizeof(buffer)); // write the buffer to the UDP stream
        udp.endPacket();                   // stop transmission
        log_i("UDP packet sent!");
        memset(buffer, 0, sizeof(buffer)); // clear the buffer
        udp.parsePacket();                 // parse the received packet
        log_i("UDP packet received!");

        if (udp.read(buffer, sizeof(buffer)) > 0)
        {
            log_i("UDP packet received!");
            log_i("%s", (char *)buffer);
        } // read the packet into the buffer
    }
}

Network network;