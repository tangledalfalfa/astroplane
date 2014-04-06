#include <stdio.h>
#include <math.h>

#include "ephstar.h"
#include "ephtime.h"
#include "ephutil.h"

/*
 * All code derived from:
 *   Astronomical Algorithms, 2nd Edition
 *   Jean Meeus
 *   Willmann-Bell, Inc.
 */

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
                struct starData *pData) {

    /* resolve apparent dispute between Meeus and International
     * Astronomical Union
     */
    lon = -lon;

    /* Julian Date */
    pData->jde = ephCalcJD(pDT);

    /* mean sidereal time in Greenwich */
    pData->theta0 = ephMSTG(pData->jde);

    /* hour angle */
    pData->ha = ephHourAngle(pData->theta0, lon, alpha);

    /* altitude, azimuth */
    ephAltAz(lat, pData->ha, delta,
            &pData->alt, &pData->az);

    /* correct for atmospheric refraction */
    pData->alt = ephAtmRef(pData->alt);
}

void ephStarDump(const struct starData *pData)
{
    printf("%-5s= %.3f days\n", "jde", pData->jde);
    printf("%-5s= %.5f degrees\n", "theta0", pData->theta0);
    printf("%-5s= %.5f degrees\n", "ha", pData->ha);
    printf("%-5s= %.5f degrees\n", "alt", pData->alt);
    printf("%-5s= %.5f degrees\n", "az", pData->az);

    return;
}
