/*
* Name: övningsprojekt
* Author: Mathea Frost
* Date: 2025-10-13
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <U8glib.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Init constants
#define PIN 6         // Digital IO pin connected to the NeoPixels.
#define NUMPIXELS 24  // Number of NeoPixels in the ring.

// Init global variables

// Construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myservo;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
  pixels.begin();  // Initialize the NeoPixel ring.

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //Attach the servo to pin 9
  myservo.attach(9);

  u8g.setFont(u8g_font_helvB10);
}

void loop() {
  oledWrite(getTime());
  servoWrite(getTemp());

  Serial.println(getTime());

  delay(200);

  DateTime now = rtc.now();


  pixelClock(now.second(), now.minute(), now.hour());
  //clock.minutes(now.minutes());
  //clock.hours(now.hours());
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return (rtc.getTemperature());
  Serial.print(rtc.getTemperature());
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
  u8g.firstPage();
  do {
    u8g.drawStr(0, 22, text.c_str());
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  myservo.write(map(getTemp(), 15, 40, 0, 180));
  Serial.println(map(getTemp(), 15, 40, 0, 180));
}

/*
* Takes the time and maps it to pixels on the LED ring to create an analog clock.
* Parameters: Values: s (seconds), m (minutes)
* Returns: void
*/

void pixelClock(int s, int m, int h) {
  pixels.setPixelColor((map(s, 0, 59, 0, 23)), pixels.Color(0, 100, 100));  // Set color to cyan.
  pixels.setPixelColor((map(m, 0, 59, 0, 23)), pixels.Color(100, 100, 0));  // Set color to yellow.
  pixels.setPixelColor((map(h, 0, 11, 0, 23)), pixels.Color(100, 0, 100));  // Set color to purple.
  pixels.show();                                                            // Update the ring with the new color.
  pixels.setPixelColor((map(s, 0, 59, 0, 23)), pixels.Color(0, 0, 0));      // Turn off the pixel.
  pixels.setPixelColor((map(m, 0, 59, 0, 23)), pixels.Color(0, 0, 0));      // Turn off the pixel.
  pixels.setPixelColor((map(h, 0, 11, 0, 23)), pixels.Color(0, 0, 0));      // Turn off the pixel.
}
