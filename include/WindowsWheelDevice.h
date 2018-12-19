#ifndef WINDOWSWHEELDEVICE_H_
#define WINDOWSWHEELDEVICE_H_

#include "BLEDevice.h"
#include "BLEHIDDevice.h"

class WindowsWheelDevice
{
  public:
    WindowsWheelDevice(std::string deviceName = "Windows Wheel Device", std::string manufacturerName = "Espressif");

    void emitWheelEvent(bool buttonPressed, int16_t rotation);
    void setBatteryLevel(uint8_t level);

  private:
    BLEServer *pServer;
    BLEHIDDevice *pHidDevice;


};     // WindowsWheelDevice
#endif /* WINDOWSWHEELDEVICE_H_ */