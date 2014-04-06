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

/* polynomial of order n in T */
/* coefficients in a[] */
/* results are anglees, in degrees */
/* value returned always within the range [0..360) */
double
ephPolyT(double t, double *a, int n)
{
    int i;
    double y;

    y = 0;
    for (i=0; i<n; i++) {
        y *= t;
        y += a[n-1-i];
    }

    return ephAngleRed(y);
}

/* ephMeanOblEcl: calculate mean obliquity of ecliptic */
/*   Laskar's formula, equation (22.3) */
/*   (epsilon0) */
double
ephMeanOblEcl(double t)
{
    static double moblecl[] = {23+(26+21.448/60)/60, -4680.93/3600,
            -1.55, 1999.25, -51.38, -249.67, -39.05, 7.12, 27.87,
            5.79, 2.45};

    return ephPolyT(t/100, moblecl, 11);
}

/* formula adopted by International Astronomical Union */
/*   (not the most accurate) */
/*   equation (22.2) */
/******
*double
*ephMeanOblEcl(double t)
*{
*    static double moblecl[] = {23+(26+21.448/60)/60, -46.8150/3600,
*            -0.00059/3600, 0.001813/3600};
*
*    return ephPolyT(t, moblecl, 4);
*}
******/

/* transformation from ecliptical to equatorial coordinates
 * input:
 *    lambda:  ecliptical longitude (degrees)
 *      beta:  ecliptical latitude (degrees)
 *   epsilon:  obliquity of the ecliptic (degrees)
 * output:
 *   alpha:  right ascension (degrees)
 *   delta:  declination (degrees)
 */
/* derived from equations (13.3), (13.4) */
void
ephEclToEqu(double lambda, double beta, double epsilon,
        double *pAlpha, double *pDelta)
{
    double n1;
    double d1;
    double a1;
    double a2;

    /* convert all to radians */
    lambda = ephDegToRad(lambda);
    beta = ephDegToRad(beta);
    epsilon = ephDegToRad(epsilon);

    n1 = sin(lambda)*cos(epsilon) - tan(beta)*sin(epsilon);
    d1 = cos(lambda);

    a1 = sin(beta)*cos(epsilon);
    a2 = cos(beta)*sin(epsilon)*sin(lambda);

    *pAlpha = ephRadToDeg(atan2(n1, d1));
    *pDelta = ephRadToDeg(asin(a1 + a2));

    /* right ascension should be [0..360) */
    if (*pAlpha < 0)
        *pAlpha += 360;

    return;
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
