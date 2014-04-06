#ifndef EPHTIME_H
#define EPHTIME_H

/*
 * All code derived from:
 *   Astronomical Algorithms, 2nd Edition
 *   Jean Meeus
 *   Willmann-Bell, Inc.
 */

/* year, month, day, hour, minute, second */
struct ymdhms {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
};

/*
 * ephCalcJD: convert ymdhms to JD (or JDE if DT is specified)
 *   input:
 *     pointer to ymdhms struct
 *   returns:
 *     JD (or JDE)
 *     (see chapters 7, 10)
 */
double ephCalcJD(struct ymdhms *pTime);

/*
 * ephCalcT: convert JDE to T
 *   input:
 *     JDE
 *   returns:
 *     T
 *     (see chapter 22, equation (22.1))
 */
/* convert JDE to T */
double ephCalcT(double jde);

/*
 * ephMSTG: calculate mean sidereal time at Greenwich
 *   input:
 *     JD
 *   returns:
 *     theta0 (degrees)
 *     (see equation (12.4))
 */
double ephMSTG(double jd);

/*
 * ephHourAngle: calculate hour angle, degrees
 *   input:
 *     theta0: sidereal time at Greenwich, degrees,
 *     lon:  observer's longitude, degrees (west is positive)
 *     alpha:  right ascension, degrees
 *   returns:
 *     H (hour angle), degrees
 *     (see chapter 13, page 92)
 */
double ephHourAngle(double theta0, double lon, double alpha);

#endif
