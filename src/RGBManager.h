/*
    SlimeVR Code is placed under the MIT license
    Copyright (c) 2022 TheDevMinerTV

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
#ifndef SLIMEVR_RGBMANAGER_H
#define SLIMEVR_RGBMANAGER_H

#include <Arduino.h>
#include "globals.h"
#include "logging/Logger.h"

#if ESP32
    #include "nvs_flash.h"
#endif

#define LEDR_CH 1
#define LEDG_CH 2 
#define LEDB_CH 3 
#define LEDR_PIN 0
#define LEDG_PIN 1 
#define LEDB_PIN 7  

namespace SlimeVR
{
    class RGBManager
    {
    public:
        uint8_t ledr;
        uint8_t ledg;
        uint8_t ledb;

        void setup();

        /*!
         *  @brief update pin status
         */
        void update();

    private:

        Logging::Logger m_Logger = Logging::Logger("RGBManager");
    };
}

#endif
