# talking-useless-box
Electronics mods to make a simple useless box talk and shade some light. This is part of a workshop I gave which was documented at that time.
Here I describe only the electronics part of the project, not the full assembly.

## Requirements
Install the following libraries in Arduino IDE using the library manager or manually using the links below:
* TMRpcm audio library available here https://github.com/TMRh20/TMRpcm
* Adafruit's NeoPixel library to drive the LEDs can be found here https://github.com/adafruit/Adafruit_NeoPixel

## Hardware setup
* Mainboard is any Arduino supported by the libraries listed above
* An SD card reader than uses SPI to communicate with the Arduino (any big internet seller with have this piece in stock for you)
* The SD card matching the SD reader (SD or microSD)
* Some NeoPixel compatible LEDs (ie WS2812 strip leds)
* A speaker
* A resistor and a transistor to drive the speaker (to avoid drawing too much current from the Arduino's output)
