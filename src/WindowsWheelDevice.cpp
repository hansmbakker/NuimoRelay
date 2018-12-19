#include "WindowsWheelDevice.h"
#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

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

WindowsWheelDevice::WindowsWheelDevice(std::string deviceName, std::string manufacturerName)
{
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    //TODO
    //pServer->setCallbacks(new MyCallbacks());

    //TODO
    //pServer->setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);
    //pServer->setSecurityCallbacks(new MySecurity());

    /*
		 * Instantiate hid device
		 */
    pHidDevice = new BLEHIDDevice(pServer);

    /*
		 * Set manufacturer name
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.manufacturer_name_string.xml
		 */
    pHidDevice->manufacturer()->setValue(manufacturerName);

    /*
		 * Set pnp parameters
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.pnp_id.xml
		 */
    //const uint8_t pnp[] = {0x01, 0x02, 0xe5, 0xab, 0xcd, 0x01, 0x10};
    pHidDevice->pnp(0x01, 0x02E5, 0xabcd, 0x0110); //->setValue((uint8_t *)pnp, sizeof(pnp));

    /*
		 * Set hid informations
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.hid_information.xml
		 */
    //const uint8_t val1[] = {0x01, 0x11, 0x00, 0x03};
    pHidDevice->hidInfo(0x00, 0x03); //->setValue((uint8_t *)val1, 4);

    /*
		 * Set report map (here is initialized device driver on client side)
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.report_map.xml
		 */
    pHidDevice->reportMap((uint8_t *)_hidReportDescriptorSurfaceDial, sizeof(_hidReportDescriptorSurfaceDial));

    /*
		 * We are prepared to start hid device services. Before this point we can change all values and/or set parameters we need.
		 * Also before we start, if we want to provide battery info, we need to prepare battery service.
		 * We can setup characteristics authorization
		 */
    pHidDevice->startServices();

    /*
		 * Its good to setup advertising by providing appearance and advertised service. This will let clients find our device by type
		 */
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(GENERIC_HID);
    pAdvertising->addServiceUUID(pHidDevice->hidService()->getUUID());
    pAdvertising->start();

    //TODO
    //BLESecurity *pSecurity = new BLESecurity();
    //pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
    //pSecurity->setCapability(ESP_IO_CAP_NONE);
    //pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    ESP_LOGD(LOG_TAG, "Advertising started!");
    //delay(1000000);
}

void WindowsWheelDevice::emitWheelEvent(bool buttonPressed, int16_t rotation)
{
    if (pHidDevice != nullptr)
    {
        Serial.println("preparing report");
        WindowsWheelReport report;

        Serial.println("adding info to report");
        report.button = buttonPressed;
        report.rotation = rotation;
        //report.xAxis = x;
        //report.yAxis = y;

        Serial.println("casting report to bytes");
        Serial.printf("report data: %8x%8x\n", ((uint8_t*)&report)[0], ((uint8_t*)&report)[1]);
        
        Serial.println("writing bytes to service");
        
        pHidDevice->inputReport(1)->setValue((uint8_t*)&report, 2);
        
        Serial.println("notifying");
        pHidDevice->inputReport(1)->notify();
    }
}

void WindowsWheelDevice::setBatteryLevel(uint8_t level)
{
    if (pHidDevice != nullptr)
    {
        pHidDevice->setBatteryLevel(level);
    }
}