#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Sun.h"

#define PI M_PI
#define RAD PI / 180

// sun calculations are based on http://aa.quae.nl/en/reken/zonpositie.html formulas

#define J1970 2440588
#define J2000 2451545

double toJulian(time_t date) {
	return (double)date / 60 / 60 / 24 - 0.5 + J1970;
}

time_t fromJulian(double j) {
	return (j + 0.5 - J1970) * 60 * 60 * 24;
}

double toDays(time_t date) {
	return toJulian(date) - J2000;
}

// general calculations for position

double E = RAD * 23.4397; // obliquity of the Earth

double rightAscension(double l, double b) {
	return atan2(sin(l) * cos(E) - tan(b) * sin(E), cos(l));
}

double declination(double l, double b) {
	return asin(sin(b) * cos(E) + cos(b) * sin(E) * sin(l));
}

double azimuth(double H, double phi, double dec) {
	return atan2(sin(H), cos(H) * sin(phi) - tan(dec) * cos(phi));
}

double altitude(double H, double phi, double dec) {
	return asin(sin(phi) * sin(dec) + cos(phi) * cos(dec) * cos(H));
}

double siderealTime(double d, double lw) {
	return RAD * (280.16 + 360.9856235 * d) - lw;
}

double astroRefraction(double h) {
	if (h < 0) // the following formula works for positive altitudes only.
		h = 0; // if h = -0.08901179 a div/0 would occur.
	// formula 16.4 of "Astronomical Algorithms" 2nd edition by Jean Meeus (Willmann-Bell, Richmond) 1998.
	// 1.02 / tan(h + 10.26 / (h + 5.10)) h in degrees, result in arc minutes -> converted to rad:
	return 0.0002967 / tan(h + 0.00312536 / (h + 0.08901179));
}

// general sun calculations

double solarMeanAnomaly(double d) {
	return RAD * (357.5291 + 0.98560028 * d);
}

double eclipticLongitude(double M) {
	double C = RAD * (1.9148 * sin(M) + 0.02 * sin(2 * M) + 0.0003 * sin(3 * M)); // equation of center
	double P = RAD * 102.9372; // perihelion of the Earth
	return M + C + P + PI;
}

// calculations for sun times

double J0 = 0.0009;

double julianCycle(double d, double lw) {
	return round(d - J0 - lw / (2 * PI));
}

double approxTransit(double Ht, double lw, double n) {
	return J0 + (Ht + lw) / (2 * PI) + n;
}

double solarTransitJ(double ds, double M, double L) {
	return J2000 + ds + 0.0053 * sin(M) - 0.0069 * sin(2 * L);
}

double hourAngle(double h, double phi, double d) {
	return acos((sin(h) - sin(phi) * sin(d)) / (cos(phi) * cos(d)));
}

double observerAngle(double height) {
	return -2.076 * sqrt(height) / 60;
}

// returns set time for the given sun altitude
double getSetJ(double h, double lw, double phi, double dec, double n, double M, double L) {
	double w = hourAngle(h, phi, dec);
	double a = approxTransit(w, lw, n);
	return solarTransitJ(a, M, L);
}

time_t hoursLater(time_t date, int h) {
	return date + h * 60 * 60;
}

Sun::Sun(time_t date, double latitude, double longitude) {
	_date = date;
	_latitude = latitude;
	_longitude = longitude;
	double d = toDays(_date);
	// calculate coordinates
	double M = solarMeanAnomaly(d);
	double L = eclipticLongitude(M);
	_declination = declination(L, 0);
	_rightAscension = rightAscension(L, 0);
	// calculate position
	double lw  = RAD * -_longitude;
	double phi = RAD * _latitude;
	double H = siderealTime(d, lw) - _rightAscension;
	_azimuth = azimuth(H, phi, _declination);
	_altitude = altitude(H, phi, _declination);
}

void Sun::calculateTimes() {
	// calculate times
	double d = toDays(_date);
	double lw  = RAD * -_longitude;
	double phi = RAD * _latitude;
	double n = julianCycle(d, lw);
	double ds = approxTransit(0, lw, n);
	double M = solarMeanAnomaly(ds);
	double L = eclipticLongitude(M);
	//double dec = declination(L, 0);
	double Jnoon = solarTransitJ(ds, M, L);
	solarNoon = fromJulian(Jnoon);
	nadir = fromJulian(Jnoon - 0.5);
	double angle = -0.833;
	double height = 0;
	double dh = observerAngle(height);
	double h0 = (angle + dh) * RAD;
	double Jset = getSetJ(h0, lw, phi, _declination, n, M, L);
	double Jrise = Jnoon - (Jset - Jnoon);
	sunrise = fromJulian(Jrise);
	sunset = fromJulian(Jset);
}

#define LAT 32.9842190
#define LNG 35.4488415



void pdate(time_t date) {
	struct tm *t;
	t = gmtime(&date);
	printf("%02d/%02d/%d %02d:%02d:%02d ", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, t->tm_sec);
}

int main() {
	struct tm t = {};
	t.tm_year = 123;
	t.tm_mon = 4;
	t.tm_mday = 8;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	time_t date = mktime(&t);
	
//	for (int x = 0; x < 6 * 23; x++) {
//		getPosition(date, LAT, LNG, &p);
//		printf("%s %f %f\n", ctime(&date), p.azimuth, p.altitude);
//		date += 10 * 60;
//	}

	for (int x = 0; x < 6 * 24; x++) {
		Sun s = Sun(date, LAT, LNG);

		//printf("%s", ctime(&date));
		pdate(date);
		printf("s.azimuth=%.f s.altitude=%.f\n", s._azimuth /2/PI*360, s._altitude /2/PI*360);
//		s.calculateTimes();
//		printf("sunrise=%s", ctime(&s.sunrise));
//		printf("sunset=%s", ctime(&s.sunset));
//		Moon m = Moon(date, LAT, LNG);
//		printf("m.azimuth=%.f m.altitude=%.f m.pa=%.f ", m._azimuth /2/PI*360, m._altitude /2/PI*360, m.parallacticAngle /2/PI*360);
//		printf("fraction=%.3f phase=%f angle=%.f\n", m.fraction, m.phase, m.angle /2/PI*360);
//		m.calculateTimes();
//		printf("moonrise=%s", ctime(&m.moonrise));
//		printf("moonset=%s", ctime(&m.moonset));

		date += 10 * 60;


	}
}
