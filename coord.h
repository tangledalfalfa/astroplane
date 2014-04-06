/*
 * Header file for coordinate transform module
 */

#ifndef _COORD_H_
#define _COORD_H_

#include "vector3.h"

/* cartesian: use v3_str from vector3 */

/* spherical */
struct crd_sph_str {
    double r;                  /* distance from origin */
    double phi;                /* zenith angle (radians) */
    double theta;              /* angle (radians) from x toward y in xy plane */
};

/*
 * public function prototypes
 */

/* cartesian to spherical */
void crd_cart2sph(const struct v3_str *crt, struct crd_sph_str *sph);
/* spherical to cartesian */
void crd_sph2cart(const struct crd_sph_str *sph, struct v3_str *crt);

#endif
