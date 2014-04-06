/*
 * coordinate transform module
 */

#include <math.h>

#include "coord.h"

#define SQR(x) ((x) * (x))

/*
 * public functions
 */

/* cartesian to spherical */
void crd_cart2sph(const struct v3_str *crt, struct crd_sph_str *sph)
{
    sph->r = sqrt(SQR(crt->x) + SQR(crt->y) + SQR(crt->z));
    sph->phi = acos(crt->z / sph->r);
    sph->theta = atan2(crt->y, crt->x);
    if (sph->theta < 0.0)
        sph->theta += 2.0 * M_PI;
}

/* spherical to cartesian */
void crd_sph2crt(const struct crd_sph_str *sph, struct v3_str *crt)
{
    crt->x = sph->r * sin(sph->phi) * cos(sph->theta);
    crt->y = sph->r * sin(sph->phi) * sin(sph->theta);
    crt->z = sph->r * cos(sph->phi);
}
