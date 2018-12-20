#include "BLEDevice.h"
#include "NuimoService.h"

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

const BLEUUID buttonCharacteristicGuid("f29b1529-cb19-40f3-be5c-7241ecb82fd2");
const BLEUUID rotationCharacteristicGuid("f29b1528-cb19-40f3-be5c-7241ecb82fd2");
const BLEUUID swipeTouchCharacteristicGuid("f29b1527-cb19-40f3-be5c-7241ecb82fd2");
const BLEUUID flyCharacteristicGuid("f29b1526-cb19-40f3-be5c-7241ecb82fd2");

NuimoService::NuimoService(BLERemoteService *nuimoRemoteService)
{
    pRemoteService = nuimoRemoteService;
}

BLEUUID NuimoService::nuimoServiceUuid = BLEUUID::fromString("f29b1525-cb19-40f3-be5c-7241ecb82fd2");

void NuimoService::setupCallbacks(notify_callback buttonCallback, notify_callback rotationCallback)
{
    BLERemoteCharacteristic *buttonCharacteristic = this->getButtonCharacteristic();
    if (buttonCharacteristic->canNotify())
    {
        buttonCharacteristic->registerForNotify(buttonCallback);
    }

    BLERemoteCharacteristic *rotationCharacteristic = this->getRotationCharacteristic();
    if (rotationCharacteristic->canNotify())
    {
        rotationCharacteristic->registerForNotify(rotationCallback);
    }
}

BLERemoteCharacteristic *NuimoService::getButtonCharacteristic()
{
    return pRemoteService->getCharacteristic(buttonCharacteristicGuid);
}

BLERemoteCharacteristic *NuimoService::getRotationCharacteristic()
{
    return pRemoteService->getCharacteristic(rotationCharacteristicGuid);
}
