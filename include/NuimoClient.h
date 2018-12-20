#pragma once

#include <BLEDevice.h>
#include "NuimoService.h"
#include "cppQueue.h"

class NuimoClient : BLEClientCallbacks
{
  public:
    NuimoClient();
    void scanForNuimos();
    void foundNuimo(BLEAdvertisedDevice *pDevice);
    void foundNuimo(BLEAddress address);
    bool connectToServer(BLEAdvertisedDevice *pDevice);
    bool connectToServer(BLEAddress address);

    void onConnect(BLEClient *pClient);
    void onDisconnect(BLEClient *pClient);

    bool isConnected();

    BLEAdvertisedDevice* pAdvertisedNuimo;
    
  private:
    BLEClient *m_pClient;
    bool mConnected;

    NuimoService *m_pNuimoService;

    BLERemoteService *getNuimoService();
};