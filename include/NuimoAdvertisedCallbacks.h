#pragma once

#include "BLEAdvertisedDevice.h"
#include "NuimoClient.h"

/**

 * Scan for BLE servers and find the first one that advertises the service we are looking for.

 */
class NuimoAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  public:
    NuimoAdvertisedDeviceCallbacks(NuimoClient *nuimoClient);
    /**
	 * Called for each advertising BLE server.
	 */
    void onResult(BLEAdvertisedDevice advertisedDevice);

  private:
    NuimoClient *m_pNuimoClient;

}; // MyAdvertisedDeviceCallbacks
