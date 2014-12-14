#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 10

// Data pin that led data will be written out over
#define DATA_PIN 11

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// setup inputs
const int brightness_pot = A0;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_brightness = 0;        // value read from the brightness pot

/*const int hue_pot = A1;  // Analog input pin that the potentiometer is attached to for hue
int sensorValue_hue = 0;        // value read from the hue pot
const int saturation_pot = A2;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_saturation = 0;        // value read from the saturation pot
*/

// ***********************************************************************************************************
// *
// *                            Power Up Init.
// *
// *
// ***********************************************************************************************************
void setup() {

	// sanity check delay - allows reprogramming if accidently blowing power w/leds
	delay(2000);

	// Change this line as needed to match the LED string type, control chip and color sequence
	FastLED.addLeds<TM1803, DATA_PIN, GBR>(leds, NUM_LEDS); // RadioShack LED String
	FastLED.setDither(false); //disable FastLED dithering, was causing a flicker at some brightness levels

	// turn off all leds
	fill_solid ( &(leds[0]), NUM_LEDS, CRGB::Black);
	FastLED.show();
	delay(1000);
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
/*	sensorValue_hue = analogRead(hue_pot);
	hue = map(sensorValue_hue, 0,1023,0,255);
	sensorValue_saturation = analogRead(saturation_pot);
	saturation = map(sensorValue_saturation,0,1023,0,255); */
	FastLED.setBrightness(brightness);
	FastLED.showColor(CHSV(hue, saturation, 255));

}
