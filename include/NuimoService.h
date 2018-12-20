#ifndef NUIMOSERVICE_H_
#define NUIMOSERVICE_H_

#include "BLEDevice.h"

class NuimoService
{
  public:
    NuimoService(BLERemoteService *nuimoRemoteService);
    void setupCallbacks(notify_callback buttonCallback, notify_callback rotationCallback);
    static BLEUUID nuimoServiceUuid;

  private:
    BLERemoteCharacteristic *getButtonCharacteristic();
    BLERemoteCharacteristic *getRotationCharacteristic();
    BLERemoteService *pRemoteService;

};     // NuimoService
#endif /* NUIMOSERVICE_H_ */
