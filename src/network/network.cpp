/*
    SlimeVR Code is placed under the MIT license
    Copyright (c) 2021 Eiren Rain

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "network.h"

bool lastBleConnected = false;
bool lastWifiConnected = false;

void Network::setUp() {
#ifdef BLE_MODE
    BLENetwork::setUp();
#else
    WiFiNetwork::setUp();
#endif
}

void Network::update(Sensor * const sensor, Sensor * const sensor2) {
#ifdef BLE_MODE
    //BLENetwork::upkeep();
    if(BLENetwork::isConnected()) {
        if(lastBleConnected == false) {
            lastBleConnected = true;
        }
        ServerConnection::update(sensor, sensor2);
    } else {
        lastBleConnected = false;
    }
#else
    WiFiNetwork::upkeep();
    if(WiFiNetwork::isConnected()) {
        if(lastWifiConnected == false) {
            lastWifiConnected = true;
            ServerConnection::resetConnection(); // WiFi was reconnected, reconnect to the server
        }
        ServerConnection::update(sensor, sensor2);
    } else {
        lastWifiConnected = false;
    }
#endif
}
