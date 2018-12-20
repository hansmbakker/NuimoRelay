#include "WindowsWheelDevice.h"
#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

static const char *LOG_TAG = "WindowsWheelDevice";

WindowsWheelDevice::WindowsWheelDevice(std::string deviceName, std::string manufacturerName)
{
    mDeviceName = deviceName;
    mManufacturerName = manufacturerName;
}

void WindowsWheelDevice::begin()
{
    pServer = BLEDevice::createServer();
    
    pServer->setCallbacks(this);

    /*
		 * Instantiate hid device
		 */
    pHidDevice = new BLEHIDDevice(pServer);

    /*
    * Setup security
    */
    // Input / Output
    _inputCharacteristic = pHidDevice->inputReport(1);
    _setAccessPermission(_inputCharacteristic);
    auto outputCharacteristic = pHidDevice->outputReport(1);
    _setAccessPermission(outputCharacteristic);

    // Add encryption to built-in characteristics and descriptors.
    _setAccessPermission(pHidDevice->deviceInfo(), 0x2a50);

    /*
		 * Set pnp parameters
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.pnp_id.xml
		 */
    //const uint8_t pnp[] = {0x01, 0x02, 0xe5, 0xab, 0xcd, 0x01, 0x10};
    pHidDevice->pnp(0x01, 0x02E5, 0xabcd, 0x0110); //->setValue((uint8_t *)pnp, sizeof(pnp));
    //pHidDevice->pnp(0x02, 0xe502, 0xa111, 0x0210);

    /*
		 * Set manufacturer name
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.manufacturer_name_string.xml
		 */
    pHidDevice->manufacturer()->setValue(mManufacturerName);

    _setAccessPermission(pHidDevice->hidService(), 0x2a4a);

    /*
		 * Set hid informations
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.hid_information.xml
		 */
    //const uint8_t val1[] = {0x01, 0x11, 0x00, 0x03};
    //pHidDevice->hidInfo(0x00, 0x03); //->setValue((uint8_t *)val1, 4);
    pHidDevice->hidInfo(0x00, 0x01);

    _setAccessPermission(pHidDevice->hidService(), 0x2a4b);
    _setAccessPermission(pHidDevice->hidService(), 0x2a4c);
    _setAccessPermission(pHidDevice->hidService(), 0x2a4e);
    _setAccessPermission(pHidDevice->batteryService(), 0x2a19);

    // Set battery level.
    pHidDevice->setBatteryLevel(100);

    /*
		 * Set report map (here is initialized device driver on client side)
		 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.report_map.xml
		 */
    pHidDevice->reportMap((uint8_t *)_hidReportDescriptorSurfaceDial, sizeof(_hidReportDescriptorSurfaceDial));

    // Disable Security.
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setCapability(ESP_IO_CAP_NONE);

    /*
		 * We are prepared to start hid device services. Before this point we can change all values and/or set parameters we need.
		 * Also before we start, if we want to provide battery info, we need to prepare battery service.
		 * We can setup characteristics authorization
		 */
    pHidDevice->startServices();
}

void WindowsWheelDevice::startAdvertising()
{
    /*
		 * Its good to setup advertising by providing appearance and advertised service. This will let clients find our device by type
		 */
    BLEAdvertising *advertising = pServer->getAdvertising();
    advertising->setAppearance(0xC003);
    advertising->addServiceUUID(pHidDevice->hidService()->getUUID());
    advertising->start();
    pServer->startAdvertising();
    ESP_LOGI(LOG_TAG, "Advertising started!");
    ESP_LOGI(LOG_TAG, "Search '%s' in your Bluetooth device list.", mDeviceName);
}

void WindowsWheelDevice::stopAdvertising()
{
    BLEAdvertising *advertising = pServer->getAdvertising();
    advertising->stop();
    ESP_LOGI(LOG_TAG, "BLE advertising has been <STOPPED>.");
}

void WindowsWheelDevice::onConnect(BLEServer *server)
{
    _connected = true;
    ESP_LOGI(LOG_TAG, "%s has been <CONNECTED>.", mDeviceName);
    stopAdvertising();
}

void WindowsWheelDevice::onDisconnect(BLEServer *server)
{
    _connected = false;
    ESP_LOGI(LOG_TAG, "%s has been <DISCONNECTED>.", mDeviceName);
    startAdvertising();
}

void WindowsWheelDevice::_setAccessPermission(BLECharacteristic *characteristic)
{
    characteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED |
                                         ESP_GATT_PERM_WRITE_ENCRYPTED);

    auto desc = characteristic->getDescriptorByUUID(BLEUUID((uint16_t)0x2904));
    if (desc)
    {
        desc->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
    }

    desc = characteristic->getDescriptorByUUID(BLEUUID((uint16_t)0x2908));
    if (desc)
    {
        desc->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
    }

    desc = characteristic->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    if (desc)
    {
        desc->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
    }
}

void WindowsWheelDevice::_setAccessPermission(BLEService *service, uint16_t uuid)
{
    _setAccessPermission(service->getCharacteristic(BLEUUID(uuid)));
}

void WindowsWheelDevice::emitWheelEvent(bool buttonPressed, int16_t rotation)
{
    ESP_LOGI(LOG_TAG, "Emitting wheel event");
    if (pHidDevice != nullptr)
    {
        ESP_LOGD(LOG_TAG, "preparing report");
        WindowsWheelReport report;

        ESP_LOGD(LOG_TAG, "adding info to report");
        report.button = buttonPressed;
        report.rotation = rotation;
        //report.xAxis = x;
        //report.yAxis = y;

        ESP_LOGD(LOG_TAG, "casting report to bytes");
        ESP_LOGD(LOG_TAG, "report data: %8x%8x\n", ((uint8_t *)&report)[0], ((uint8_t *)&report)[1]);

        ESP_LOGD(LOG_TAG, "writing bytes to service");

        _inputCharacteristic->setValue((uint8_t *)&report, 2);

        ESP_LOGD(LOG_TAG, "notifying");
        _inputCharacteristic->notify();
    }
}

void WindowsWheelDevice::setBatteryLevel(uint8_t level)
{
    if (pHidDevice != nullptr)
    {
        pHidDevice->setBatteryLevel(level);
    }
}