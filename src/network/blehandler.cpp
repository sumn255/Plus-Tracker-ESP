#include "blehandler.h"
#include "globals.h"
#include "network.h"
#include "packets.h"
#include "logging/Logger.h"
#include "GlobalVars.h"
#include <vector>
//#include "udpclient.cpp"
extern uint8_t ble_incomingPacket[128]; // buffer for incoming packets
extern int ble_len;
extern bool is_ble_recv;

std::vector<uint8_t> ble_packet;
uint64_t ble_packetNumber = 0;
uint8_t ble_buf[8];


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//设备的MAC地址

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool isBleConnected = false;
//bool oldDeviceConnected = false;
//uint32_t value = 0;
uint8_t ble_rssi;
extern unsigned long last_rssi_sample;
SlimeVR::Logging::Logger bleHandlerLogger("BLEHandler");
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

template <typename T>
unsigned char * convert_to_chars(T src, unsigned char * target)
{
    union uwunion
    {
        unsigned char c[sizeof(T)];
        T v;
    } un;
    un.v = src;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        target[i] = un.c[sizeof(T) - i - 1];
    }
    return target;
}

template <typename T>
T convert_chars(unsigned char * const src)
{
    union uwunion
    {
        unsigned char c[sizeof(T)];
        T v;
    } un;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        un.c[i] = src[sizeof(T) - i - 1];
    }
    return un.v;
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      isBleConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
      isBleConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      //std::string rxValue = pCharacteristic->getValue();
        ble_len = pCharacteristic->getLength();
        memcpy(ble_incomingPacket, pCharacteristic->getData(), ble_len);
        is_ble_recv = true;
    }
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      ble_rssi = advertisedDevice.getRSSI();
    }
}; 

bool BLENetwork::isConnected(){
    return isBleConnected;
}

void BLENetwork::setUp() {
    bleHandlerLogger.info("Setting up BLE");
    //set mac
    //esp_base_mac_addr_set(mac);
    // Create the BLE Device
    BLEDevice::init("SlimeVR FBT Tracker");
    // Create the BLE Server
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
}

void BLENetwork::upkeep(){

    if(isBleConnected) {
        bleHandlerLogger.warn("Connection to BLE Client lost, reconnecting...");
        isBleConnected = false;
    }

    if(!isBleConnected) {
        //onConnected();
        statusManager.setStatus(SlimeVR::Status::WIFI_CONNECTING, false);
        isBleConnected = true;
        //hadWifi = true;
        //wifiHandlerLogger.info("Connected successfully to SSID '%s', ip address %s", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
        return;
    } else {
        if(millis() - last_rssi_sample >= 2000) {
            last_rssi_sample = millis();
            Network::sendSignalStrength(ble_rssi);
        }
    }
    return;
}

namespace BLE_DataTransfer {

    void cleanPacket(){
        ble_packet.resize(0);
    }

    void transferPacket(){
        pCharacteristic->setValue(ble_packet.data(), ble_packet.size());
        pCharacteristic->notify();
    }

    void setPacketType(uint8_t type) {
        setByte(0);
        setByte(0);
        setByte(0);
        setByte(type);
    }

    void setPacketNumber() {
        uint64_t pn = ble_packetNumber++;
        setLong(pn);
    }

    void setFloat(float f) {
        convert_to_chars(f, ble_buf);
        for (size_t n = 0; n < sizeof(f); n++)
		{
			ble_packet.push_back(ble_buf[n]);
		}
    }

    void setByte(uint8_t c) {
        ble_packet.push_back(c);
    }

    void setInt(int i) {
        convert_to_chars(i, ble_buf);
        for (size_t n = 0; n < sizeof(i); n++)
		{
			ble_packet.push_back(ble_buf[n]);
		}
    }

    void setLong(uint64_t l) {
        convert_to_chars(l, ble_buf);
        for (size_t n = 0; n < sizeof(l); n++)
		{
			ble_packet.push_back(ble_buf[n]);
		}
    }

    void setBytes(const uint8_t * c, size_t length) {
        for (size_t n = 0; n < length; n++)
		{
			ble_packet.push_back(*(c+n));
		}
    }

    void setShortString(const char * str) {
        uint8_t size = strlen(str);
        setByte(size); // String size
        setBytes((const uint8_t *) str, size); // Firmware version string
    }
    
    void setLongString(const char * str) {
        int size = strlen(str);
        setInt(size); // String size
        setBytes((const uint8_t *) str, size); // Firmware version string
    }

}