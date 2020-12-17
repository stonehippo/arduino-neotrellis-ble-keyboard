#include <bluefruit.h>
#include "Adafruit_NeoTrellis.h"

BLEDis bledis;
BLEHidAdafruit blehid;

Adafruit_NeoTrellis trellis;

char keys[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
char deviceName[] = "BLE Keyboard";
char manufacturerName[] = "Stonehippo";

void setup() {
  // Turn on the serial connection so we can get messages
  Serial.begin(115200);
  Serial.println("Goooooooo");
  
  // Turn on the Trellis buttons
  if (!trellis.begin()) {
    Serial.println("trellis is not working");
    // Something went wrong, so loop forever
    while(1);
  } else {
    Serial.println("trellis is working");
  }

  // Light up the trellis buttons
  // For each of the buttons and LEDs on the Trellis...
  for (uint16_t i=0; i < trellis.pixels.numPixels(); i++) {
    // ...set the color of the next pixel...
    trellis.pixels.setPixelColor(i, trellis.pixels.Color(127,127,127));
    // ...and update all the pixels...
    trellis.pixels.show();
    // ... and wait for a little bit, so we can see the animation
    delay(50);
  }

  // Now turn off all the LEDs
  for (uint16_t i=0; i < trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(50);
  }
  
  // Turn on Bluetooth
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName(deviceName);

  // Use the Device Information Service to give some info about us
  bledis.setManufacturer(manufacturerName);
  bledis.setModel(deviceName);
  bledis.begin();

  // Start up HID (act like a keyboard)
  blehid.begin();

  // Get the Trellis ready to send keys
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, callback);
  }

  // Bluetooth advertising, so this device can be found
  startAdvertising();
  Serial.print("Bluetooth is advertising");
}

void startAdvertising() {
  // Set up advertising data
  // Make it so devices can find us
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  // Let devices that find us know our transmission strength
  Bluefruit.Advertising.addTxPower();
  // Let devices that find us know that we are a keyboard
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  // Advertise that we have the HID service
  Bluefruit.Advertising.addService(blehid);
  // Advertise our name
  Bluefruit.Advertising.addName();

  // Start advertising
  // If a device disconnects, start advertising again
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  // Keep advertising as long as we're not connected
  Bluefruit.Advertising.start(0);
}

TrellisCallback callback(keyEvent event){
  // Check is the pad pressed? If so turn on that LED and send a key
  if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    trellis.pixels.setPixelColor(event.bit.NUM, trellis.pixels.Color(127,127,127)); //on rising
    blehid.keyPress(keys[event.bit.NUM]);
  } else if (event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
  // or is the pad released? If so, turn off that LED and stop sending the key
    trellis.pixels.setPixelColor(event.bit.NUM, 0);
    blehid.keyRelease();
  }

  // Update the LEDs
  trellis.pixels.show();

  return 0;
}

void loop() {
  // Keep checking for presses on the Trellis
  trellis.read();
  delay(20);
}
