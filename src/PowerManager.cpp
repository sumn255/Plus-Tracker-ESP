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

#include "PowerManager.h"
#include "GlobalVars.h"
#include "status/Status.h"

namespace SlimeVR
{
    void PowerManager::setup()
    {
    #ifdef VKEY_PIN
        pinMode(VKEY_PIN, OUTPUT);
        digitalWrite(VKEY_PIN,LOW);
        m_Logger.info("Virtual key Plugin loaded !");
    #endif
    }

    void PowerManager::update()
    {
        unsigned long time = millis();
        unsigned long diff = time - timeStampLast;

        // Don't tick the PowerManager *too* often
        if ((keyStateLast == 1 && diff < 1000)
          ||(keyStateLast == 0 && diff < 1000))
        {
            return;
        }
    #ifdef VKEY_PIN
        if (keyStateLast == 1)
        {
            digitalWrite(VKEY_PIN,LOW);
            keyStateLast = 0;
            //m_Logger.info("Set key unPressed!");
        }
        else
        {
            digitalWrite(VKEY_PIN,HIGH);
            keyStateLast = 1;
            //m_Logger.info("Set key Pressed!");
        }
    #endif
        timeStampLast = time;
    }
}
