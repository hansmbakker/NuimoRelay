#pragma once

#include "BLEDevice.h"
#include "BLEHIDDevice.h"

typedef struct
{
    uint16_t button : 1;
    uint16_t rotation : 15;
    //int8_t xAxis;
    //int8_t yAxis;
} WindowsWheelReport;

static const uint8_t _hidReportDescriptorSurfaceDial[] = {
    // Integrated Radial Controller TLC
    0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
    0x09, 0x0e,       // USAGE (System Multi-Axis Controller)
    0xa1, 0x01,       // COLLECTION (Application)
    0x85, 1,          //   REPORT_ID (Radial Controller)
    0x05, 0x0d,       //   USAGE_PAGE (Digitizers)
    0x09, 0x21,       //   USAGE (Puck)
    0xa1, 0x00,       //   COLLECTION (Physical)
    0x05, 0x09,       //     USAGE_PAGE (Buttons)
    0x09, 0x01,       //     USAGE (Button 1)
    0x95, 0x01,       //     REPORT_COUNT (1)
    0x75, 0x01,       //     REPORT_SIZE (1)
    0x15, 0x00,       //     LOGICAL_MINIMUM (0)
    0x25, 0x01,       //     LOGICAL_MAXIMUM (1)
    0x81, 0x02,       //     INPUT (Data,Var,Abs)
    0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
    0x09, 0x37,       //     USAGE (Dial)
    0x95, 0x01,       //     REPORT_COUNT (1)
    0x75, 0x0f,       //     REPORT_SIZE (15)
    0x55, 0x0f,       //     UNIT_EXPONENT (-1)
    0x65, 0x14,       //     UNIT (Degrees, English Rotation)
    0x36, 0xf0, 0xf1, //     PHYSICAL_MINIMUM (-3600)
    0x46, 0x10, 0x0e, //     PHYSICAL_MAXIMUM (3600)
    0x16, 0xf0, 0xf1, //     LOGICAL_MINIMUM (-3600)
    0x26, 0x10, 0x0e, //     LOGICAL_MAXIMUM (3600)
    0x81, 0x06,       //     INPUT (Data,Var,Rel)
    // 0x09, 0x30,       //     USAGE (X)
    // 0x75, 0x10,       //     REPORT_SIZE (16)
    // 0x55, 0x0d,       //     UNIT_EXPONENT (-3)
    // 0x65, 0x13,       //     UNIT (Inch,EngLinear)
    // 0x35, 0x00,       //     PHYSICAL_MINIMUM (0)
    // 0x46, 0xc0, 0x5d, //     PHYSICAL_MAXIMUM (24000)
    // 0x15, 0x00,       //     LOGICAL_MINIMUM (0)
    // 0x26, 0xff, 0x7f, //     LOGICAL_MAXIMUM (32767)
    // 0x81, 0x02,       //     INPUT (Data,Var,Abs)
    // 0x09, 0x31,       //     USAGE (Y)
    // 0x46, 0xb0, 0x36, //     PHYSICAL_MAXIMUM (14000)
    // 0x81, 0x02,       //     INPUT (Data,Var,Abs)
    // 0x05, 0x0d,       //     USAGE_PAGE (Digitizers)
    // 0x09, 0x48,       //     USAGE (Width)
    // 0x36, 0xb8, 0x0b, //     PHYSICAL_MINIMUM (3000)
    // 0x46, 0xb8, 0x0b, //     PHYSICAL_MAXIMUM (3000)
    // 0x16, 0xb8, 0x0b, //     LOGICAL_MINIMUM (3000)
    // 0x26, 0xb8, 0x0b, //     LOGICAL_MAXIMUM (3000)
    // 0x81, 0x03        //     INPUT (Cnst,Var,Abs)
    0xc0, //   END_COLLECTION
    0xc0  // END_COLLECTION
};

class WindowsWheelDevice : public BLEServerCallbacks
{
public:
  // Creates a new instance of `WindowsWheelDevice`.
  WindowsWheelDevice(std::string deviceName = "Windows Wheel Device", std::string manufacturerName = "Espressif");

  // Initializes the BLE HID server.
  void begin();

  // Returns a boolean that indicates whether the Keyboard has been connected.
  bool isConnected();

  // Starts BLE advertising.
  void startAdvertising();

  // Stops BLE advertising.
  void stopAdvertising();

  // Implements BLEServerCallbacks interface
  void onConnect(BLEServer *server);
  void onDisconnect(BLEServer *server);

  void emitWheelEvent(bool buttonPressed, int16_t rotation);
  void setBatteryLevel(uint8_t level);

private:
  std::string mDeviceName;
  std::string mManufacturerName;
  void _setAccessPermission(BLECharacteristic *characteristic);
  void _setAccessPermission(BLEService *service, uint16_t uuid);

  bool _connected;

  BLEServer *pServer;
  BLEHIDDevice *pHidDevice;
  BLECharacteristic *_inputCharacteristic;
};
