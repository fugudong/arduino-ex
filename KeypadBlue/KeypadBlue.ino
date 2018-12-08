/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include <CurieBLE.h>

BLEService keypadService("180F"); // BLE Battery Service

// BLE Battery Level Characteristic"
BLEUnsignedCharCharacteristic keypadValChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);     // remote clients will be able to
// get notifications if this characteristic changes
char prevCustomKey = 0, customKey;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);   // initialize the LED on pin 13 to indicate when a central is connected

  // begin initialization
  BLE.begin();

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("KeyPadCli");
  BLE.setAdvertisedService(keypadService);  // add the service UUID
  keypadService.addCharacteristic(keypadValChar); // add the battery level characteristic
  BLE.addService(keypadService);   // Add the BLE Battery service
  keypadValChar.setValue((int)prevCustomKey);   // initial value for this characteristic

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}
  
void loop(){
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(13, HIGH);

    // check the battery level every 200ms
    // as long as the central is still connected:
    while (central.connected()) {
        updateKeypadVal();
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(13, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

int char_to_int(char key)
{
  if((key>='0') && (key<='9'))
  return (key-'0');

  if((key>='A') && (key<='D'))
  return (key-'A');
}

void updateKeypadVal() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
   customKey = customKeypad.getKey();

  if (customKey != prevCustomKey) {      // if the battery level has changed
    if (customKey){
      Serial.print("Current key: "); // print it
      Serial.println(customKey);
      keypadValChar.setValue(customKey);  // and update the battery level characteristic
    }
    prevCustomKey = customKey;           // save the level for next comparison
  }
}
