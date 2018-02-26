// Included libraries for the SD card, audio playing abilities and LEDs
#include <SD.h>
#include <TMRpcm.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Physical connections for SD card reader and LEDs
#define SD_ChipSelectPin 10
#define LEDS_CTRL_PIN    6

// Flags to prevent simultaneous interrupts
bool isTriggered = false;
bool canTrigger = true;

// Declare instances for audio and LEDs strip
TMRpcm audio;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, LEDS_CTRL_PIN, NEO_GRB + NEO_KHZ800);

// Interrupt that triggers sound and LEDs
ISR(INT1_vect) {
  if (canTrigger) {
    canTrigger = false;
    isTriggered = true; 
  }
}

// General setup
void setup(){
  // Configure INT1
  DDRD  &= ~(1 << DDD3);  // Set PD3 as input
  PORTD |= (1 << PORTD3); // Enable pull-up on PD3
  EICRA |= (1 << ISC11);    // set INT1 to trigger on falling edge
  EIMSK |= (1 << INT1);     // Turns on INT1

  // LEDs initialization
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // LEDs intensity
  strip.setBrightness(50);

  // Speaker is connected to PIN 9
  audio.speakerPin = 9;

  // SD card init
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    return;
  }

  // Set audio volume
  audio.setVolume(7);
}

// Main loop
void loop(){
  // Detect when box is open
  if (isTriggered) {
    isTriggered = false;
    // Lights up the LEDs with green color
    // Color code is RGB-based (Red Green Blue), range is 0 -> 255
    colorWipe(strip.Color(0, 255, 0), 50);
    // Play audio file which resides on the SD card
    audio.play("6.wav");
    // Wait for audio file to be read
    while (audio.isPlaying());
    // Switch off the LEDs
    colorWipe(strip.Color(0, 0, 0), 50);
    canTrigger = true;
  }

  // Color examples
  //colorWipe(strip.Color(255, 0, 0), 50); // Rouge
  //colorWipe(strip.Color(0, 255, 0), 50); // Vert
  //colorWipe(strip.Color(0, 0, 255), 50); // Bleu
  
  // Theatre Chase Mode
  //theaterChase(strip.Color(127, 127, 127), 50); // Blanc
  //theaterChase(strip.Color(127, 0, 0), 50); // Rouge
  //theaterChase(strip.Color(0, 0, 127), 50); // Bleu

  // Rainbows
  //rainbow(20);
  //theaterChaseRainbow(50);
}



/***************************************************************************
 * DO NOT MODIFY THE CODE BELOW !! (or do so if you want to hack a bit :-) )
 ***************************************************************************/



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
