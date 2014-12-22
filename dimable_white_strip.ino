/*  This is for a dimable lighting strip for our travel trailer running off the 12vdc coach system.
this version is configured for two Radio Shack led strips of different lengths, one is 10 clusters and
the other is 8 clusters. They are set up in a single LED array.

This uses the FastLED library that can control many different types of addressable leds and it should be
possible to even mix makes and types of arrays if you use a different data pin for each strip.

Your mileage may vary and it is not my fault if your computer ceases to exist, you gain a cat, etc.
*/

#include <FastLED.h>

// how many led strips  This is not actually used anywhere, it is a reference to help setup the strips and array
#define NUM_LED_STRIPS 2
// How many leds are in EACH strip?
#define NUM_LEDS1 10
#define NUM_LEDS2 8

// Data pin that led data will be written out over
#define DATA_PIN1 10
#define DATA_PIN2 11
#define Total_LEDS  NUM_LEDS1+NUM_LEDS2
// This is an array of leds.  One item for each led in your strip.
CRGB leds[Total_LEDS];

// setup inputs
const int brightness_pot = A0;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_brightness = 0;        // value read from the brightness pot
const int hue_pot = A1;  // Analog input pin that the potentiometer is attached to for hue
int sensorValue_hue = 0;        // value read from the hue pot
const int saturation_pot = A2;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_saturation = 0;        // value read from the saturation pot
#define WHITE_MODE 2  // digital input pin for white mode
#define BLUE_MODE 4  // digital input pin for blue mode
// set color correction and temperature of the strips
#define color_correction TypicalSMD5050
#define  color_temperature DirectSunlight

// ***********************************************************************************************************
// *
// *                            Power Up Init.
// *
// *
// ***********************************************************************************************************
void setup() {

	// sanity check delay - allows reprogramming if accidently blowing power w/leds
	delay(2000);
        pinMode (WHITE_MODE, INPUT_PULLUP);
        pinMode (BLUE_MODE, INPUT_PULLUP);

	// Change this line as needed to match the LED string type, control chip and color sequence
	FastLED.addLeds<TM1803, DATA_PIN1, GBR>(leds, 0, NUM_LEDS1).setCorrection( color_correction ); // RadioShack LED String
	FastLED.addLeds<TM1803, DATA_PIN2, GBR>(leds, NUM_LEDS1,NUM_LEDS2).setCorrection( color_correction ); //RadioShack LED String
	FastLED.setDither(false); //disable FastLED dithering, was causing a flicker at some brightness levels

	// turn off all leds
	fill_solid ( &(leds[0]),Total_LEDS, CRGB::Black);
	FastLED.show();
	delay(500);
}

// ***********************************************************************************************************
// *
// *                            Main Loop
// *
// *
// ***********************************************************************************************************
void loop()
{
	static uint8_t hue = 0;
	static uint8_t brightness= 48;
	static uint8_t saturation = 0;
	sensorValue_brightness = analogRead(brightness_pot);
	brightness = map(sensorValue_brightness, 0, 1023, 2, 255);
	sensorValue_hue = analogRead(hue_pot);
	hue = map(sensorValue_hue, 0,1023,0,255);
	sensorValue_saturation = analogRead(saturation_pot);
	saturation = map(sensorValue_saturation,0,1023,0,255);
        int sensorValue_WHITE_MODE = digitalRead(WHITE_MODE);
        int sensorValue_BLUE_MODE = digitalRead(BLUE_MODE);
        FastLED.setTemperature( color_temperature );
        if (sensorValue_WHITE_MODE == LOW){
          saturation = 0;
        }
        if (sensorValue_BLUE_MODE == LOW) {
          fill_solid( &(leds[0]),Total_LEDS,CRGB(0,0,255) ); 
          FastLED.setBrightness(brightness);
          FastLED.show();
        }
        else {
        FastLED.setBrightness(brightness);
	FastLED.showColor(CHSV(hue, saturation, 255));
	}

}
