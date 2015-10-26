#include "adf4602.h"

void cgmi_get()
{
    WIFI.println("Maple Oscilloscope STM32F4 Discovery");
    WIFI.println("OK");
}

void gain_set(const String& value)
{
    int gain = value.toInt();
    TunerGain(gain);
}

void arfcn_set(const String& value)
{
    int arfcn = value.toInt();
    TunerFreq(arfcn);
}