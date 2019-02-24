#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLE2904.h>
#include <WiFi.h>

#include "WPAConnectionService.hpp"

BLEServer *pServer = NULL;
WPAConnectionService *pWPAService = NULL;

bool waitingConnectWifi = false;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Connected device");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Disconnect device");
    }
};

class WiFiConnectionServiceCallbacks: public WPAConnectionServiceCallbacks
{
    void onRequestConnect(const std::string ssid, const std::string password)
    {
        Serial.printf("Trying connect to %s.\n", ssid.c_str());
        waitingConnectWifi = true;
        WiFi.begin(ssid.c_str(), password.c_str());
    }
    void onRequestDisconnect()
    {
        Serial.println("Dissconnect Wi-Fi");
        WiFi.disconnect();
    }
};

void setup()
{
    Serial.begin(115200);

    // Create the BLE Device
    BLEDevice::init("ESP32_WiFi_config_test");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    pWPAService = new WPAConnectionService(pServer);
    pWPAService->setCallback(new WiFiConnectionServiceCallbacks());

    // Start the service
    pWPAService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(WPA_CONNECTION_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection...");
}

void loop()
{

    if (waitingConnectWifi && WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("Connect to %s, IP Addr: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
        waitingConnectWifi = false;
    }

    if (deviceConnected)
    {
        delay(3);
        // TODO
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);
        pServer->startAdvertising();
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
