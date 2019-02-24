#ifndef ___CHARACTERISTIC_H___
#define ___CHARACTERISTIC_H___

#include <BLEServer.h>

#define WPA_CONNECTION_SERVICE_UUID "6a1c63be-71ac-4d8d-91b1-8b66af6c3193"
#define SSID_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define Password_CHARACTERISTIC_UUID "0a0afd34-f565-416a-a57f-4a652fed52eb"
#define Connect_CHARACTERISTIC_UUID "40b4cb5a-739b-49d3-b230-6e16e469e93f"
#define IpAddr_CHARACTERISTIC_UUID "79995725-db40-40f7-9c1d-a4ff7116c230"
#define Status_CHARACTERISTIC_UUID "f51f39f3-8fa2-4f9e-846f-00b36053ac0e"

class WPAConnectionServiceCallbacks
{
  public:
    virtual void onRequestConnect(const std::string ssid, const std::string password);
    virtual void onRequestDisconnect();
};

class WPAConnectionService
{

  public:
    WPAConnectionService(BLEServer *server);

    void start();
    void stop();
    void setCallback(WPAConnectionServiceCallbacks *callback);

  private:
    BLEService *mService;
    BLECharacteristic *mSSIDCharacteristic;
    BLECharacteristic *mPasswordCharacteristic;
    BLECharacteristic *mConnectCharacteristic;
    BLECharacteristic *mIpAddrCharacteristic;
    BLECharacteristic *mStatusCharacteristic;
    std::string mPassword;
    std::string mSSID;
    WPAConnectionServiceCallbacks *mCallback;

    void onWriteSSID(BLECharacteristic *);
};

#endif
