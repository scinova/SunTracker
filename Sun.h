//#ifndef Sun_h
//#define Sun_h
#include <time.h>

class Sun {
	public:
		Sun(time_t date, double latitude, double longitude);
		void calculateTimes();
		double _azimuth;
		double _altitude;
		double _declination;
		double _rightAscension;
		time_t sunrise;
		time_t sunset;
		time_t nadir;
		time_t solarNoon;
	private:
		time_t _date;
		double _latitude;
		double _longitude;
};

class Moon {
	public:
		Moon(time_t date, double latitude, double longitude);
		void calculateTimes();
		double _azimuth;
		double _altitude;
		double distance;
		double parallacticAngle;
		double angle;
		double fraction;
		double phase;
		double _declination;
		double _rightAscension;
		time_t moonrise;
		time_t moonset;
		bool alwaysUp;
		bool alwaysDown;
	private:
		time_t _date;
		double _latitude;
		double _longitude;
};

typedef struct {
	double azimuth;
	double altitude;
} SunPosition;

typedef struct {
	time_t solarNoon;
	time_t nadir;
	time_t sunrise;
	time_t sunset;
} SunTimes;


typedef struct {
	double declination;
	double rightAscension;
} SunCoords;

SunCoords sunCoords(double d);

// calculates sun position for a given date and latitude/longitude
SunPosition getPosition(time_t date, double lat, double lng);

SunTimes getTimes(time_t date, double lat, double lng, double height=0);

typedef struct {
	double rightAscension;
	double declination;
	double distance;
} MoonCoordinates;

MoonCoordinates moonCoords(double d);

typedef struct {
	double azimuth;
	double altitude;
	double distance;
	double parallacticAngle;
} MoonPosition;

MoonPosition getMoonPosition(time_t date, double lat, double lng);

typedef struct {
	double fraction;
	double phase;
	double angle;
} MoonIllumination;

typedef struct {
	time_t moonrise;
	time_t moonset;
	bool alwaysUp;
	bool alwaysDown;
} MoonTimes;

MoonTimes getMoonTimes(time_t date, double lat, double lng);
//#endif
