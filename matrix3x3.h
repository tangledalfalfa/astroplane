/*
 * Header file for 3x3 matrix module
 */

#ifndef _MATRIX3X3_H_
#define _MATRIX3X3_H_

#include "vector3.h"

struct m3x3_str {
    double a1;
    double a2;
    double a3;
    double b1;
    double b2;
    double b3;
    double c1;
    double c2;
    double c3;
};

/*
 * public function prototypes
 */

/* determinant */
double m3x3_det(const struct m3x3_str *m);
/* transpose */
void m3x3_tran(struct m3x3_str *m);
/* inverse */
void m3x3_inv(struct m3x3_str *m);
/* m = m + n */
void m3x3_add(struct m3x3_str *m, const struct m3x3_str *n);
/* m = m - n */
void m3x3_sub(struct m3x3_str *m, const struct m3x3_str *n);
/* m = s * m */
void m3x3_mul(struct m3x3_str *m, double s);
/* u = u * m */
void m3x3_vmul(struct v3_str *u, const struct m3x3_str *m);
/* m = s / m */
void m3x3_div(struct m3x3_str *m, double s);
void m3x3_print(const struct m3x3_str *m);
void m3x3_inv_test(void);

#endif
