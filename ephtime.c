#include <stdio.h>

#include "ephtime.h"
#include "ephutil.h"

/*
 * All code derived from:
 *   Astronomical Algorithms, 2nd Edition
 *   Jean Meeus
 *   Willmann-Bell, Inc.
 */

/*
 * ephCalcJD: Calculate Julian Day.
 *   Derived from equation (7.1)
 */
/* Note:  this is valid for the Gregorian calendar only, not
          the Julian calendar */
/* Note:  if day is provided in UT, result will be JD,
          if day is provided as TD, result will be JDE */
double
ephCalcJD(struct ymdhms *pTime)
{
    double fDay;
    int y;
    int m;
    int a;
    int b;

    fDay = pTime->day + (pTime->hour + (pTime->minute
            + pTime->second/60.0)/60.0)/24.0;

    y = pTime->year;
    m = pTime->month;
    if (m <= 2) {
        y--;
        m += 12;
    }


    a = y / 100;
    b = 2 - a + a/4;

    return (int)(365.25 * (y+4716)) + (int)(30.6001 * (m + 1))
            + fDay + b - 1524.5;
}

/*
 * ephCalcT: Calculate T, centuries from Epoch J2000.0 (JDE 2451545.0)
 *   derived from equation (22.1)
 */
double
ephCalcT(double jde)
{
    return (jde - 2451545.0)/36525.0;
}

/* ephMSTG: mean sidereal time at Greenwich (theta0, degrees) */
/*   derived from equation (12.4) */
double
ephMSTG(double jd)
{
    double t;

    t = ephCalcT(jd);

    return ephAngleRed(280.46061837 + 360.98564736629 * (jd - 2451545.0)
            + t*t*(0.000387933 - t/38710000.0));
}

/* ephHourAngle: calculate hour angle, degrees */
/* input:
 *   theta0: sidereal time at Greenwich, degrees,
 *   lon:  observer's longitude, degrees (west is positive)
 *   alpha:  right ascension, degrees
 */
double
ephHourAngle(double theta0, double lon, double alpha)
{
    return ephAngleRed(theta0 - lon - alpha);
}
