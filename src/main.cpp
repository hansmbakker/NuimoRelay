#include <Arduino.h>

#include "BLEDevice.h"
#include "WindowsWheelDevice.h"
#include "NuimoClient.h"
#include "cppQueue.h"

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

static const char *LOG_TAG = "Main";

typedef struct strNuimoEvent
{
  bool button;
  short rotation;
} NuimoEvent;

//Create and initialize event queue
Queue nuimoQueue(sizeof(NuimoEvent), 10, FIFO);
Queue batteryQueue(sizeof(short), 10, FIFO);

//Create Nuimo Client
NuimoClient *nuimoClient;
//Create Windows Wheel Device
WindowsWheelDevice *windowsWheelDevice;

static void my_gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* param) {
	ESP_LOGW(LOG_TAG, "custom gattc event handler, event: %d", (uint8_t)event);
        if(event == ESP_GATTC_DISCONNECT_EVT) {
                Serial.print("Disconnect reason: "); 
                Serial.println((int)param->disconnect.reason);
        }
}

void setup()
{
  //Initialize Serial
  Serial.begin(115200);

  //Initialize bluetooth
  BLEDevice::setCustomGattcHandler(my_gattc_event_handler);
  BLEDevice::init("Windows Wheel Device");
    
  nuimoClient = new NuimoClient();

  sleep(3);

  //Create Windows Wheel device
  windowsWheelDevice = new WindowsWheelDevice();
  windowsWheelDevice->begin();
  windowsWheelDevice->startAdvertising();

  //Setup BLE Scanner
  //nuimoClient->scanForNuimos();
  // BLEAddress address = BLEAddress("d3:ce:97:9d:b3:a7");
  // nuimoClient->foundNuimo(address);


  // NuimoEvent evt = {true, 3135};
  // NuimoEvent evt2 = {false, -235};
  // NuimoEvent evt3 = {true, 458};
  // nuimoQueue.push(&evt);
  // nuimoQueue.push(&evt2);
  // nuimoQueue.push(&evt3);

  

  ESP_LOGI(LOG_TAG, "Setup done");
} // End of setup.

//notification:
//  add event to queue

//connect to device:
//  register notifications

// This is the Arduino main loop function.
void loop()
{

  //if device found and should connect
  //  then connect to device

  //if event queue contains items
  //  for each item in queue
  while (!nuimoQueue.isEmpty())
  {
    //    pop item in queue
    NuimoEvent evt;
    nuimoQueue.pop(&evt);
    //    emit event
    Serial.printf("Button: %s | Rotation: %i\n", evt.button ? "true" : "false", evt.rotation);
    windowsWheelDevice->emitWheelEvent(evt.button, evt.rotation);
  }

  while (!batteryQueue.isEmpty())
  {
    //    pop item in queue
    short batteryLevel;
    nuimoQueue.pop(&batteryLevel);
    //    emit event
    Serial.printf("Battery level: %u\n", batteryLevel);
    windowsWheelDevice->setBatteryLevel(batteryLevel);
  }

  Serial.println("Pressing button");
  windowsWheelDevice->emitWheelEvent(true, 0);
  sleep(1);
  Serial.println("Releasing button");
  windowsWheelDevice->emitWheelEvent(false, 0);
  sleep(1);

} // End of loop