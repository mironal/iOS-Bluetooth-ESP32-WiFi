#include "functional"
#include "WPAConnectionService.hpp"
#include <BLEService.h>
#include <BLE2904.h>

using CharacteristicCB = std::function<void(BLECharacteristic *)>;

class LambdaCharacteristicCallback : public BLECharacteristicCallbacks
{

  public:
    LambdaCharacteristicCallback(CharacteristicCB read,
                                 CharacteristicCB write)
    {
        mRead = read;
        mWrite = write;
    }

  private:
    CharacteristicCB mRead;
    CharacteristicCB mWrite;

    void onRead(BLECharacteristic *c)
    {
        if (mRead)
        {
            mRead(c);
        }
    }

    void onWrite(BLECharacteristic *c)
    {
        if (mWrite)
        {
            mWrite(c);
        }
    }
};

WPAConnectionService::WPAConnectionService(BLEServer *server)
{
    mSSID = std::string();
    mPassword = std::string();

    BLE2904 *utf8Descriptor = new BLE2904();
    utf8Descriptor->setFormat(BLE2904::FORMAT_UTF8);

    BLE2904 *boolDescriptor = new BLE2904();
    boolDescriptor->setFormat(BLE2904::FORMAT_BOOLEAN);

    BLE2904 *uint8Descriptor = new BLE2904();
    uint8Descriptor->setFormat(BLE2904::FORMAT_UINT8);

    mService = server->createService(WPA_CONNECTION_SERVICE_UUID);

    mSSIDCharacteristic = mService->createCharacteristic(
        SSID_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    mSSIDCharacteristic->addDescriptor(utf8Descriptor);

    mSSIDCharacteristic->setCallbacks(new LambdaCharacteristicCallback(NULL, [this](BLECharacteristic *c) {
        mSSID = c->getValue();
    }));

    mPasswordCharacteristic = mService->createCharacteristic(
        Password_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    mPasswordCharacteristic->addDescriptor(utf8Descriptor);
    mPasswordCharacteristic->setCallbacks(new LambdaCharacteristicCallback(NULL, [this](BLECharacteristic *c) {
        mPassword = c->getValue();
    }));

    mConnectCharacteristic = mService->createCharacteristic(
        Connect_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    mConnectCharacteristic->addDescriptor(boolDescriptor);

    mConnectCharacteristic->setCallbacks(new LambdaCharacteristicCallback(NULL, [this](BLECharacteristic *c) {
        if (!mCallback)
        {
            return;
        }

        auto connect = c->getData();
        if (*connect)
        {
            if (mSSID.length() > 0 && mPassword.length() > 0)
            {
                mCallback->onRequestConnect(mSSID, mPassword);
                // clear
                mSSID = "";
                mPassword = "";
            }
        }
        else
        {
            mCallback->onRequestDisconnect();
        }
    }));

    mIpAddrCharacteristic = mService->createCharacteristic(
        IpAddr_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ);
    mIpAddrCharacteristic->addDescriptor(utf8Descriptor);

    mStatusCharacteristic = mService->createCharacteristic(
        Status_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ);
    mStatusCharacteristic->addDescriptor(uint8Descriptor);
}

void WPAConnectionService::onWriteSSID(BLECharacteristic *c)
{
}

void WPAConnectionService::start()
{
    mService->start();
}

void WPAConnectionService::stop()
{
    mService->stop();
}

void WPAConnectionService::setCallback(WPAConnectionServiceCallbacks *callback)
{
    mCallback = callback;
}
