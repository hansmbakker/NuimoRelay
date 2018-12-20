#include <esp_log.h>
#include "NuimoClient.h"
#include "NuimoAdvertisedCallbacks.h"
#include "BLEDevice.h"
#include "NuimoService.h"

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

static const char *LOG_TAG = "NuimoClient";

void buttonCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    ESP_LOGI(LOG_TAG, "Received button data");
}

void rotationCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    ESP_LOGI(LOG_TAG, "Received rotation data");
}

NuimoClient::NuimoClient()
{
    m_pClient = BLEDevice::createClient();
    ESP_LOGI(LOG_TAG, " - Created client");

    m_pClient->setClientCallbacks(this);
    ESP_LOGI(LOG_TAG, " - Setup callbacks");
}

void NuimoClient::scanForNuimos()
{
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 30 seconds.
    ESP_LOGI(LOG_TAG, "Scanning for nuimos");
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new NuimoAdvertisedDeviceCallbacks(this));
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
}

void NuimoClient::foundNuimo(BLEAddress address)
{
    mConnected = false;
    if (connectToServer(address))
    {
        ESP_LOGI(LOG_TAG, " - Connected to device");
        mConnected = true;
        m_pNuimoService = new NuimoService(getNuimoService());
        m_pNuimoService->setupCallbacks(buttonCallback, rotationCallback);
    }
}
void NuimoClient::foundNuimo(BLEAdvertisedDevice *pDevice)
{
    mConnected = false;
    if (connectToServer(pDevice))
    {
        ESP_LOGI(LOG_TAG, " - Connected to device");
        mConnected = true;
        m_pNuimoService = new NuimoService(getNuimoService());
        m_pNuimoService->setupCallbacks(buttonCallback, rotationCallback);
    }
}

void NuimoClient::onConnect(BLEClient *pClient)
{
    mConnected = true;
}
void NuimoClient::onDisconnect(BLEClient *pClient)
{
    mConnected = false;
    //this->scanForNuimos();
    BLEAddress address = BLEAddress("d3:ce:97:9d:b3:a7");
    this->foundNuimo(address);
}

bool NuimoClient::isConnected()
{
    return mConnected;
}

bool NuimoClient::connectToServer(BLEAdvertisedDevice *pDevice)
{
    ESP_LOGI(LOG_TAG, "Forming a connection to %s", pDevice->getAddress().toString().c_str());

    // Connect to the remove BLE Server.
    m_pClient->connect(pDevice);
    ESP_LOGI(LOG_TAG, " - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = getNuimoService();
    if (pRemoteService == nullptr)
    {
        ESP_LOGE(LOG_TAG, "Failed to find our service UUID: %s", NuimoService::nuimoServiceUuid.toString().c_str());
        return false;
    }
    ESP_LOGI(LOG_TAG, " - Found our service");

    return true;
}

bool NuimoClient::connectToServer(BLEAddress address)
{
    ESP_LOGI(LOG_TAG, "Forming a connection to %s", address.toString().c_str());

    // Connect to the remove BLE Server.
    m_pClient->connect(address, BLE_ADDR_TYPE_PUBLIC);
    ESP_LOGI(LOG_TAG, " - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = getNuimoService();
    if (pRemoteService == nullptr)
    {
        ESP_LOGE(LOG_TAG, "Failed to find our service UUID: %s", NuimoService::nuimoServiceUuid.toString().c_str());
        return false;
    }
    ESP_LOGI(LOG_TAG, " - Found our service");

    return true;
}

BLERemoteService *NuimoClient::getNuimoService()
{
    return m_pClient->getService(NuimoService::nuimoServiceUuid);
}