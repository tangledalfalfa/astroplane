/*
 * 3D vector module
 */

#include <math.h>

#include "vector3.h"

/*
 * public functions
 */

/* scalar functions */

/* magnitude (Euclidean norm) */
double v3_mag(const struct v3_str *u)
{
    return sqrt((u->x * u->x) +
                (u->y * u->y) +
                (u->z * u->z));
}

/* dot product */
double v3_dot(const struct v3_str *u, const struct v3_str *v)
{
    return ((u->x * v->x) +
            (u->y * v->y) +
            (u->z * v->z));
}

/* triple scalar product (equivalent to determinant) */
double v3_3prod(const struct v3_str *u, const struct v3_str *v,
                 const struct v3_str *w)
{
    struct v3_str t = *v;

    v3_cross(&t, w);
    return v3_dot(u, &t);
}

/* returns smaller angle between vectors u, v (radians) */
double v3_angle(const struct v3_str *u, const struct v3_str *v)
{
    return acos(v3_dot(u, v) /
                ((v3_mag(u)) * (v3_mag(v))));
}

/* vector functions */

/* u = u + v */
void v3_add(struct v3_str *u, const struct v3_str *v)
{
    u->x += v->x;
    u->y += v->y;
    u->z += v->z;
}

/* u = u - v */
void v3_sub(struct v3_str *u, const struct v3_str*v)
{
    u->x -= v->x;
    u->y -= v->y;
    u->z -= v->z;
}

/* u = u * s */
void v3_mul(struct v3_str *u, double s)
{
    u->x *= s;
    u->y *= s;
    u->z *= s;
}

/* u = u / s */
void v3_div(struct v3_str *u, double s)
{
    u->x /= s;
    u->y /= s;
    u->z /= s;
}

/* convert to u to unit vector */
void v3_unit(struct v3_str *u)
{
    v3_div(u, v3_mag(u));
}

/* reverse vector u */
void v3_rev(struct v3_str *u)
{
    v3_mul(u, -1.0);
}

/* u = u cross v */
void v3_cross(struct v3_str *u, const struct v3_str *v)
{
    double tx, ty;

    tx = (u->y * v->z) - (u->z * v->y);
    ty = (u->z * v->x) - (u->x * v->z);
    u->z = (u->x * v->y) - (u->y * v->x);
    u->y = ty;
    u->x = tx;
}

/*
 * distance from line to plane:
 *  - p_line: point at start of line
 *  - u: vector in direction of line
 *  - p_plane: point on plane
 *  - n: vector normal to plane
 */
double v3_dist_line_plane(const struct v3_str *p_line,
                          const struct v3_str *u,
                          const struct v3_str *p_plane,
                          const struct v3_str *n)
{
    struct v3_str p = *p_plane;

    v3_sub(&p, p_line);
    return v3_dot(&p, n) / v3_dot(u, n);
}
