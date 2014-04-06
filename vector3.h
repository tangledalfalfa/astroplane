/*
 * Header file for 3D vector module
 */

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

struct v3_str {
    double x;
    double y;
    double z;
};

/*
 * public function prototypes
 */

/* scalar functions */

/* magnitude (Euclidean norm) */
double v3_mag(const struct v3_str *u);
/* triple scalar product */
double v3_3prod(const struct v3_str *u, const struct v3_str *v,
                 const struct v3_str *w);
/* dot product */
double v3_dot(const struct v3_str *u, const struct v3_str *v);
/* returns smaller angle between vectors u, v (radians) */
double v3_angle(const struct v3_str *u, const struct v3_str *v);

/* vector functions */

/* u = u + v */
void v3_add(struct v3_str *u, const struct v3_str *v);
/* u = u - v */
void v3_sub(struct v3_str *u, const struct v3_str *v);
/* u = u * s */
void v3_mul(struct v3_str *u, double s);
/* u = u / s */
void v3_div(struct v3_str *u, double s);
/* convert to u to unit vector */
void v3_unit(struct v3_str *u);
/* reverse vector u */
void v3_rev(struct v3_str *u);
/* u = u cross v */
void v3_cross(struct v3_str *u, const struct v3_str *v);
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
                          const struct v3_str *n);

#endif
