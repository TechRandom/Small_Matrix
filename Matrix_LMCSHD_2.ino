#include <FastLED.h>

#define PIN 4
#define BRIGHTNESS 30     // Out of 255

#define w  32
#define h  16
#define NUMMATRIX (w * h)

CRGB matrixleds[NUMMATRIX];
char rawData[NUMMATRIX * 2]; // Two Bytes per pixel

void setup() {
  FastLED.addLeds<NEOPIXEL,PIN>(matrixleds, NUMMATRIX); 
  Serial.begin(115200);
}

void loop() {
  uint8_t data = Serial.read();
  if (data == 0x05){
    uint8_t width = w, height = h;
    Serial.println(width);
    Serial.println(height);
  }
  else if (data == 0x42){
    Serial.readBytes(rawData, NUMMATRIX * 2);
    // Loop through LED array
    for (int i = 0; i < NUMMATRIX; i++){
      // Seperate payload data into individual colors
      // Get the first 5 bits of byte 1
      int red = *(rawData + (2*i)) >> 3;
      // Get the last 3 bits of byte 1 and the first 2 bits of byte 2
      int green = ((*(rawData + (2*i)) & 0x07) << 2) | (*(rawData + (2*i) + 1) >> 6);
      // Get the last 5 bits of byte 
      int blue = *(rawData + (2*i) + 1) & 0x1F;
      // Limit each color to the defined brightness
      matrixleds[i].r = map(red, 0, 0x1F, 0, BRIGHTNESS);
      matrixleds[i].g = map(green, 0, 0x1F, 0, BRIGHTNESS);
      matrixleds[i].b = map(blue, 0, 0x1F, 0, BRIGHTNESS);
    }
    FastLED.show();
    Serial.write(0x06); //acknowledge
  }
}
