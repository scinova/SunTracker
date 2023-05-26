#include <LiquidCrystal.h>
#include <TinyGPSPlus.h>
#include "Sun.h"
//#include <Arduino.h>
#include <time.h>
#include <math.h>

#define AZIMUTH_SERVO_PIN 6
#define ALTITUDE_SERVO_PIN 7

#define LCD_RS 14
#define LCD_EN 27
#define LCD_D4 26
#define LCD_D5 25
#define LCD_D6 33
#define LCD_D7 32

#define LAT 32.9842190
#define LNG 35.4488415

double _latitude = LAT;
double _longitude = LNG;
time_t _date;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setAzimuth(double v) {
	// servo.set(v) // -180~180
}

void setAltitude(double v) {
	// servo.set(v) // -180~180
}

void setup() {
	Serial.begin(115200);
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("00:00:00 00/00/0000");

	// init gps
	// init motors
}

void loop() {
	struct tm t = {};
	t.tm_year = 123;
	t.tm_mon = 4;
	t.tm_mday = 8;
	t.tm_hour = 18;
	t.tm_min = 0;
	t.tm_sec = 0;
	time_t date = mktime(&t);

	Sun sun = Sun(_date, _latitude, _longitude);
	sun.calculateTimes();
	double azimuth = sun._azimuth;
	double altitude = sun._altitude;
	if (altitude > 0) {
		setAzimuth(azimuth);
		setAltitude(altitude);
	} else {
		setAzimuth(0);
		setAltitude(0);
	}

	char buf[16];
	sprintf(&buf[0], "A:%03.f H:%03.f", azimuth/2/M_PI*360, altitude/2/M_PI*360);

	//sprintf(buf, "%06ld", millis() / 1000);
	lcd.setCursor(0, 1);
	lcd.print(buf);

	delay(1000);
}

void test() {
	struct tm t = {};
	t.tm_year = 123;
	t.tm_mon = 4;
	t.tm_mday = 8;
	t.tm_hour = 18;
	t.tm_min = 0;
	t.tm_sec = 0;
	//Serial.println(asctime(&t));
	time_t date = mktime(&t);
	for (int x = 0; x < 6 * 23; x++) {
		Serial.print(ctime(&date));
		//SunPosition p = getPosition(date, LAT, LNG);
		Sun sun = Sun(date, LAT, LNG);
		sun.calculateTimes();
		date += 10 * 60;
	}
}
