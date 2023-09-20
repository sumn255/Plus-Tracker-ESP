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

#include "RGBManager.h"
#include "GlobalVars.h"
#include "status/Status.h"

namespace SlimeVR
{
    SlimeVR::Logging::Logger logger("SerialCommands");

    void RGBManager::setup()
    {

        nvs_handle_t rgb_handle;
        esp_err_t err;

        err = nvs_open("storage", NVS_READWRITE, &rgb_handle);
        if (err != ESP_OK) {
            logger.info("Error (%s) opening NVS handle!", esp_err_to_name(err));
        } else {
            logger.info("Done\n");
        }
        err = nvs_get_u8(rgb_handle, "ledr", &ledr);
        err = nvs_get_u8(rgb_handle, "ledg", &ledg);
        err = nvs_get_u8(rgb_handle, "ledb", &ledb);
        err = nvs_commit(rgb_handle);
        nvs_close(rgb_handle);

        /* 1. 使用ledc通道1，设置频率为1kHz，分辨率为10位 */
        //double f = ledcSetup(led_ch, 1000, 10);  
        //Serial.print("F=");Serial.println(f); /* 打印实际设置的频率*/
        ledcSetup(LEDR_CH, 1000, 10);
        ledcSetup(LEDG_CH, 1000, 10);
        ledcSetup(LEDB_CH, 1000, 10);

        /* 2. 将LED引脚绑定到ledc通道1 */
        ledcAttachPin(LEDR_PIN, LEDR_CH);
        ledcAttachPin(LEDG_PIN, LEDG_CH);
        ledcAttachPin(LEDB_PIN, LEDB_CH);

        /* 3. 设置ledc通道占空比为512/1024=50% */
        ledcWrite(LEDR_CH, 1023 - 4 * ledr);//1023
        ledcWrite(LEDG_CH, 1023 - 4 * ledg);//1023
        ledcWrite(LEDB_CH, 1023 - 4 * ledb);//1023

    }

    void RGBManager::update()
    {


    }
}
