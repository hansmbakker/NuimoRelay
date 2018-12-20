#include <esp_log.h>
#include "NuimoAdvertisedCallbacks.h"
#include "NuimoService.h"
#include "NuimoClient.h"

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

static const char *LOG_TAG = "NuimoAdvertisedDeviceCallbacks";

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
NuimoAdvertisedDeviceCallbacks::NuimoAdvertisedDeviceCallbacks(NuimoClient *nuimoClient)
{
    m_pNuimoClient = nuimoClient;
}

/**
	 * Called for each advertising BLE server.
	 */
void NuimoAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    ESP_LOGI(LOG_TAG, "Advertised Device: %s", advertisedDevice.toString().c_str());
    //if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(NuimoService::nuimoServiceUuid))
    if (advertisedDevice.getName() == "Nuimo")
    {
        advertisedDevice.getScan()->stop();
        ESP_LOGI(LOG_TAG, "Found our device!  address: %s", advertisedDevice.getAddress().toString().c_str());
        m_pNuimoClient->pAdvertisedNuimo = new BLEAdvertisedDevice(advertisedDevice);
    } // Found our server
} // onResult
