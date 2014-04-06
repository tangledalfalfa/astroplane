/*
 * calculate altitude, azimuth given right ascension, declination
 */
#ifndef EPHSTAR_H
#define EPHSTAR_H

#include "ephtime.h"

/* The following structure is populated by ephStarPos() */
/* all angles are in degrees */
struct starData {
    double jde;        /* Julian Ephemeris Day */
    double theta0;     /* mean sidereal time at Greenwich */
    double ha;         /* hour angle */
    double alt;        /* altitude */
    double az;         /* azimuth */
};

/*
 * ephStarPos: calculate altitude, azimuth (etc.) of the Star
 *   input:
 *     pDT: pointer to struct specifying dynamical time
 *     lat: latitude, degrees (North is positive)
 *     lon: longitude, degrees (East is positive)
 *   alpha: right ascension
 *   delta: declination
 *  output:
 *    starData struct is populated
 */
void ephStarPos(struct ymdhms *pDT, double lat, double lon,
                double alpha, double delta,
                struct starData *pData);

/*
 * ephStarDump: print out starData struct
 *   input:
 *     pointer to structure
 */
void ephStarDump(const struct starData *pData);

#endif
