#ifndef EPHUTIL_H
#define EPHUTIL_H

/*
 * All code derived from:
 *   Astronomical Algorithms, 2nd Edition
 *   Jean Meeus
 *   Willmann-Bell, Inc.
 */

/*
 * ephAngleRed: reduce angle (in degrees) to within
 *   the range [0..360)
 *   input:
 *     angle, in degrees
 *   returns:
 *     angle, degrees, between 0 and 360
 */
double ephAngleRed(double deg);

/*
 * ephDegToRad: convert degrees to radians
 *   input: angle in degrees
 *   returns: angle in radians
 */
double ephDegToRad(double deg);

/*
 * ephRadToDeg: convert radians to degrees
 *   input: angle in radians
 *   returns: angle in degrees
 */
double ephRadToDeg(double rad);

/* various trig functions, in degrees */
double ephSin(double theta);
double ephCos(double theta);
double ephTan(double theta);
double ephASin(double x);
double ephATan2(double y, double x);

/*
 * ephPolyT:  polynomial calculation, in T
 *   input:
 *     t: time, in centuries since epoch J2000.0
 *     a: array of n coefficients
 *     n: size of array a
 *   returns:
 *     evaluated polynomial, in degrees, in range [0..360)
 */
double ephPolyT(double t, double *a, int n);

/*
 * ephMeanOblEcl: calculate mean obliquity of the Ecliptic
 *   input:  T
 *   returns: angle in degrees
 *   (see equation (22.3))
 */
double ephMeanOblEcl(double t);

/*
 * ephEclToEqu: transform ecliptical coordinates to equatorial
 *   input:
 *     lambda: ecliptical longitude, degrees,
 *     beta: ecliptical latitude, degrees,
 *     epsilon: obliquity of the ecliptic, degrees
 *   output:
 *     alpha: right ascension, degrees
 *     delta: declination, degrees
 *     (see equations (13.3), (13.4))
 */
void ephEclToEqu(double lambda, double beta, double epsilon,
        double *pAlpha, double *pDelta);

/*
 * ephAltAz: calculation of local horizontal coordinates
 *   input:
 *     lat: latitude, degrees
 *     ha: hour angle, degrees
 *     delta: declination, degrees
 *   output:
 *     alt: altitude, degrees
 *     az: azimuth, degrees East of North
 *     (see equations (13.3), (13.4))
 */
void ephAltAz(double lat, double ha, double delta,
        double *pAlt, double *pAz);

/*
 * functions to help translate Alt/Az to spherical coordinates
 *   (compatible with coord module)
 */
/* alt to polar (zenith) angle, degrees */
double ephAltToPhi(double alt);
/* azimuth: east of north to north of east */
double ephAzToTheta(double az);

/*
 * ephAtmRef: convert true altitude to apparent altitude
 *            (atmospheric refraction correction)
 *   input: true altitude (degrees)
 *   output: apparent altitude (degrees)
 *   (see Meeus, chapter 16)
 */
double ephAtmRef(double h);

#endif
