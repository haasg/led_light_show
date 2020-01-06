#include <FastLED.h>

#define WHOLE 96
#define HALF 48
#define QUARTER 24
#define EIGHTH 12
#define SIXTEENTH 6

CRGB leds[150];

byte data;
byte midi_clock = 0xf8; // MIDI Clock Byte
int mclock = 1; // Time through current measure 0 -> 96     
int measure = 1; // Current measure

void setup() 
{
  Serial.begin(115200); // Start Serial Port for Hairless MIDI
  FastLED.addLeds<WS2812, 2, GRB>(leds, 150); // Init LED Strip
}

void loop() 
{
  
  if(Serial.available() > 0) // Check if we got a byte
  {
    data = Serial.read(); // Read in the byte
    if(data == midi_clock) // Make sure the byte is a midi clock byte
    {
      mclock += 1;
  
      if(mclock >= (24 * 4))
      {
        measure += 1;
        mclock = 0;
      }
      
      SetStripOff();
  
      if(measure >= 1 && measure <= 4)
      {
        SetStrip(mclock, QUARTER*0, QUARTER, 0, 150, 0, 0, 255);
        SetStrip(mclock, QUARTER*1, QUARTER, 0, 150, 255, 0, 0);
        SetStrip(mclock, QUARTER*2, QUARTER, 0, 150, 0, 255, 0);
        SetStrip(mclock, QUARTER*3, QUARTER, 0, 150, 255, 255, 0);
      }
      if(measure >= 5 && measure <= 8)
      {
        SetStrip(mclock, QUARTER*0, QUARTER, 0, 30, 0, 0, 255);
        SetStrip(mclock, QUARTER*1, QUARTER, 30, 60, 255, 0, 0);
        SetStrip(mclock, QUARTER*2, EIGHTH, 60, 90, 0, 255, 0);
        SetStrip(mclock, QUARTER*2 + EIGHTH, EIGHTH, 90, 120, 255, 255, 0);
        SetStrip(mclock, QUARTER*3, QUARTER, 120, 150, 255, 0, 255);
      }
  
  
      
      FastLED.show();
    }
  }  
}

void SetStripOff()
{
  for(int i = 0; i < 150; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
}


void SetStrip(int time_current, int time_start, int time_length, int first_pixel, int last_pixel, int red, int green, int blue)
{
  if(time_current >= time_start && time_current < time_start+time_length)
  {
    for(int i = first_pixel; i < last_pixel; i++)
    {
      leds[i] = CRGB(red, green, blue);
    }
  }
}
