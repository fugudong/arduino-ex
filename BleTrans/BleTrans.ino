/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>


BLEService transferService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service


// create switch characteristic and allow remote device to read and write
BLECharCharacteristic recvCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLECharCharacteristic notifyCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); // allows remote device to get notifications

void setup() {
  Serial.begin(9600);

  // begin initialization
  BLE.begin();

  // set the local name peripheral advertises
  BLE.setLocalName("BleTransfer");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(transferService);

  // add the characteristics to the service
  transferService.addCharacteristic(recvCharacteristic);
  transferService.addCharacteristic(notifyCharacteristic);

  // add the service
  BLE.addService(transferService);

  recvCharacteristic.setValue(0);
  notifyCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();


  //if (buttonChanged) {
    // button state changed, update characteristics
   // ledCharacteristic.setValue(buttonValue);
   // buttonCharacteristic.setValue(buttonValue);
 // }

  if (recvCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    if (recvCharacteristic.value()) {
      Serial.println("LED on");
    } else {
      Serial.println("LED off");
    }
  }
}
