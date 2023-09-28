#ifndef SLIMEVR_BLEHANDLER_H_
#define SLIMEVR_BLEHANDLER_H_


#ifdef ESP32
    #include <BLEDevice.h>
    #include <BLEServer.h>
    #include <BLEUtils.h>
    #include <BLE2902.h>
#endif

namespace BLENetwork {
    bool isConnected();
    void setUp();
    void upkeep();

}

#endif

