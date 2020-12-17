# arduino-neotrellis-ble-keyboard

A basic BLE HID keyboard using an [Adafruit NeoTrellis](https://learn.adafruit.com/adafruit-neotrellis) and [Feather nrf52](https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide).

I used the [nRF52832-based Feather](https://www.adafruit.com/product/3406), but the more powerful nRF52840 Express would also work (but if you go that route, you'll have to remove the shrouded header for the SWD interface on the board to fit it into the NeoTrellis case). Also note that you can use CircuitPython if you move to the nRF52840, as I did with the [Zoom Keyboard](https://github.com/stonehippo/zoom-keyboard).

You can design and build your own case, or you can get the handy [NeoTrellis enclosure kit](https://www.adafruit.com/product/4339), plus a few extras like a Li-Ion battery and a switch (see the [Zoom Keyboard BOM](https://github.com/stonehippo/zoom-keyboard#bill-of-materials) for details).

## Dependencies

- [Adafruit nRF52 Board Support](https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/arduino-bsp-setup)
- [Adafruit Seesaw Library](https://github.com/adafruit/Adafruit_Seesaw) - provides an API for the NeoTrellis

## How it works

The Feather uses the BLEHidAdafruit library to mimic a Bluetooth keyboard. When a pad on the NeoTrellis is pressed, the device sends a character corresponding to the position of pad, with a values from the `keys[]` array.

For example, pressing on pad 16 (in the upper left corner if you're using the NeoTrellis enclosure), the 16th value, the keyboard will send the character 'f'. I used a single character corresponding to a hex value. You can easily modify this code to have the pads send other key values, strings, or command sequences.

Note that this implementation will send the value as long as a key is pressed. If you want to only have it exactly once, move the `blehid.keyRelease()` in the callback from the code handling the rising event, and place it after the `blehid.keyPress()` in the falling event handler.
