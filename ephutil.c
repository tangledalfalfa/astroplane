#include <math.h>

#include "ephutil.h"

/*
 * All code derived from:
 *   Astronomical Algorithms, 2nd Edition
 *   Jean Meeus
 *   Willmann-Bell, Inc.
 */

/*
 * ephAngleRed: reduce large angle to [0..360) degrees
 *   (Astronomical Algorithms, chapter 47)
 */
double
ephAngleRed(double deg)
{
    double theta;

    theta =  deg - 360 * (int)(deg/360);
    return (theta < 0) ? theta+360 : theta;
}

double
ephDegToRad(double deg)
{
    return deg * M_PI / 180.0;
}

double
ephRadToDeg(double rad)
{
    return rad * 180.0 / M_PI;
}

/* versions of trig functions for ephemeris calculations */
/*   all angles measured in degrees */

double
ephSin(double theta)
{
    return sin(ephDegToRad(theta));
}

double
ephCos(double theta)
{
    return cos(ephDegToRad(theta));
}

double
ephTan(double theta)
{
    return tan(ephDegToRad(theta));
}

double
ephASin(double x)
{
    return ephRadToDeg(asin(x));
}

double
ephATan2(double y, double x)
{
    return ephRadToDeg(atan2(y, x));
}

/* transformation of local horizontal coordinates
 * input:
 *   lat: observer's latitude, degrees (north is positive)
 *   ha: hour angle, degrees
 *   delta: declination, degrees
 * output:
 *   alt: altitude, degrees above horizon
 *   az: azimuth, degrees east of north
 */
/* derived from equations (13.5), (13.6) */
void
ephAltAz(double lat, double ha, double delta,
        double *pAlt, double *pAz)
{
    double n1;
    double d1;
    double a1;
    double a2;

    /* convert all to radians */
    lat = ephDegToRad(lat);
    ha = ephDegToRad(ha);
    delta = ephDegToRad(delta);

    n1 = sin(ha);
    d1 = cos(ha)*sin(lat) - tan(delta)*cos(lat);

    a1 = sin(lat)*sin(delta);
    a2 = cos(lat)*cos(delta)*cos(ha);

    *pAlt = ephRadToDeg(asin(a1 + a2));
    *pAz = ephRadToDeg(atan2(n1, d1));
    /*
     * Meeus defines azimuth to start in the south,
     * we want it to start in the north.
     */
    *pAz = ephAngleRed(*pAz + 180);

    return;
}

/*
 * functions to help translate Alt/Az to spherical coordinates
 *   (compatible with coord module)
 */

/* alt to polar (zenith) angle, degrees */
double
ephAltToPhi(double alt)
{
    return 90.0 - alt;
}

/* azimuth: east of north to north of east */
double
ephAzToTheta(double az)
{
    return ephAngleRed(90.0 - az);
}

/*
 * ephAtmRef:  correct for atmospheric refraction.
 *   converts from true altitude (h) to apparent altitude (h0)
 *   (both in degrees)
 *   Derived from equation (16.4)
 *     (threshhold derived from equation (16.3))
 */
double
ephAtmRef(double h)
{
    double r;  /* refraction, minutes of arc */

    /* use eqn (16.3) with h0 set to 0 to determine true altitude
       of object on horizon */
    if (h*60 > -1/ephTan(7.31/4.4))
        r = 1.02/ephTan(h + 10.3/(h + 5.11)) + 0.0019279;
    else
        r = 0;

    return h + r/60;
}
