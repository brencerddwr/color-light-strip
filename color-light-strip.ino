/*  This is for a dimable lighting strip for our travel trailer running off the 12vdc coach system.
This has three modes blue, white and variable.  The blue mode is for treatment of the medical condition SAD,
the white mode is for working and general use, and the variable mode is for mood lighting.

This is configured for two Radio Shack led strips of different lengths, one is 10 clusters (3 leds controlled by one chip)
and the other is 8 clusters. They are set up in a single LED array.

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
#define Total_LEDS  NUM_LEDS1+NUM_LEDS2

// Data pins that led data will be written out over
#define DATA_PIN1 10  //pin for 10 segment strip
#define DATA_PIN2 11  //pin for 8 segment strip
#define WHITE_MODE 4  // digital input pin for white mode, pulled low to enable
#define BLUE_MODE 5  // digital input pin for blue mode, pulled low to enable
// set color correction and temperature of the strips
#define color_correction TypicalSMD5050  //FastLED color correction (see color.h for options)
#define  color_temperature DirectSunlight //FastLED color temperature (see color.h for options)

// This is an array of leds.  One item for each led in your strip.
CRGB leds[Total_LEDS];

// setup inputs
const int brightness_pot = A0;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_brightness = 0;        // value read from the brightness pot
const int hue_pot = A1;  // Analog input pin that the potentiometer is attached to for hue
int sensorValue_hue = 0;        // value read from the hue pot
const int saturation_pot = A2;  // Analog input pin that the potentiometer is attached to for brightness
int sensorValue_saturation = 0;        // value read from the saturation pot
int sensorValue_WHITE_MODE = HIGH;  //initialize and set white mode high
int sensorValue_BLUE_MODE = HIGH;  // initialize and set blue mode high

// ***********************************************************************************************************
// *
// *                            Power Up Init.
// *
// ***********************************************************************************************************
void setup() {
	
	// sanity check delay - allows reprogramming if accidentally blowing power w/leds
	delay(2000);
	pinMode (WHITE_MODE, INPUT_PULLUP); //setup data pin for input with internal pull up resistor enabled
	pinMode (BLUE_MODE, INPUT_PULLUP); //setup data pin for input with internal pull up resistor enabled

	// Change these lines as needed to match the LED string type, control chip and color sequence
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
// ***********************************************************************************************************
void loop()
{
	static uint8_t hue = 0;  //setup hue variable for chsv
	static uint8_t brightness= 48;  //setup brightness variable
	static uint8_t saturation = 0;  // setup saturation variable
	sensorValue_brightness = analogRead(brightness_pot);  //read brightness pot
	brightness = map(sensorValue_brightness, 0, 1023, 0, 255); // map brightness value
	if (brightness <10)  // force limit on minimum brightness to deal with red pixels working at lower levels than the others
	{brightness =10;
	}
	sensorValue_hue = analogRead(hue_pot);  // read hue pot
	hue = map(sensorValue_hue, 0,1023,0,255);  //map hue pot
	sensorValue_saturation = analogRead(saturation_pot);  //read saturation value
	saturation = map(sensorValue_saturation,0,1023,0,255);  //map saturation value
	if (saturation <75)  // force limit on desaturation to deal with red pixels working at lower levels than the others
	{ saturation=75;
	}
	sensorValue_WHITE_MODE = digitalRead(WHITE_MODE); //check white mode pin
	sensorValue_BLUE_MODE = digitalRead(BLUE_MODE); //check blue mode pin
	FastLED.setTemperature( color_temperature );  // set color temperature
	if (sensorValue_WHITE_MODE == LOW){ //check if white mode is enabled
		saturation = 0;  //set saturation to 0, any hue will be white
	}
	if (sensorValue_BLUE_MODE == LOW) {  //check if blue mode is enabled
		fill_solid( &(leds[0]),Total_LEDS,CRGB(0,0,255) ); //set all leds to blue  only
		FastLED.setBrightness(brightness); // adjust brightness to selected value
		FastLED.show();
	}
	else {
		FastLED.setBrightness(brightness);
		FastLED.showColor(CHSV(hue, saturation, 255));
	}

}
