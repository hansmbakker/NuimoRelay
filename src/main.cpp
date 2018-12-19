#include <Arduino.h>

#include "BLEDevice.h"
#include "WindowsWheelDevice.h"
#include "cppQueue.h"

typedef struct strNuimoEvent {
  bool  button;
	short rotation;
  short batteryLevel;
} NuimoEvent;

//Create and initialize event queue
Queue	q(sizeof(NuimoEvent), 10, FIFO);

//Create Windows Wheel Device
WindowsWheelDevice* windowsWheelDevice;

void setup() {
  //Initialize Serial
  Serial.begin(115200);

  //Setup BLE Scanner
  NuimoEvent evt = {true, 3135, 49};
  NuimoEvent evt2 = {false, -235, 35};
  NuimoEvent evt3 = {true, 458, 23};
  q.push(&evt);
  q.push(&evt2);
  q.push(&evt3);

  //Create Windows Wheel device
  windowsWheelDevice = new WindowsWheelDevice();

  sleep(5);

  Serial.println("Setup done");
} // End of setup.

//notification:
//  add event to queue

//connect to device:
//  register notifications


// This is the Arduino main loop function.
void loop() {

  //if device found and should connect
  //  then connect to device

  //if event queue contains items
  //  for each item in queue
  while(!q.isEmpty()){
    
    //    pop item in queue
      NuimoEvent evt;
      q.pop(&evt);
    //    emit event
      Serial.printf("Button: %s | Rotation: %i | Battery level: %u\n", evt.button ? "true" : "false", evt.rotation, evt.batteryLevel);
      windowsWheelDevice->emitWheelEvent(evt.button, evt.rotation);
      windowsWheelDevice->setBatteryLevel(evt.batteryLevel);
  }
  
} // End of loop