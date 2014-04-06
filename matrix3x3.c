/*
 * 3x3 matrix module
 */

#include <stdio.h>
#include <math.h>

#include "matrix3x3.h"

/*
 * private functions
 */

static double det2x2(double a1, double a2, double b1, double b2)
{
    return ((a1 * b2)
            - (a2 * b1));
}

/*
 * public functions
 */

/* determinant */
double m3x3_det(const struct m3x3_str *m)
{
    return ((m->a1 * m->b2 * m->c3)
            - (m->a1 * m->b3 * m->c2)
            - (m->a2 * m->b1 * m->c3)
            + (m->a2 * m->b3 * m->c1)
            + (m->a3 * m->b1 * m->c2)
            - (m->a3 * m->b2 * m->c1));
}

/* transpose */
void m3x3_tran(struct m3x3_str *m)
{
    double t;

    t = m->b1;
    m->b1 = m->a2;
    m->a2 = t;

    t = m->c1;
    m->c1 = m->a3;
    m->a3 = t;

    t = m->c2;
    m->c2 = m->b3;
    m->b3 = t;
}

/* inverse */
void m3x3_inv(struct m3x3_str *m)
{
    struct m3x3_str t;
    double det;

    det = m3x3_det(m);

    t.a1 = det2x2(m->b2, m->b3, m->c2, m->c3);
    t.b1 = det2x2(m->b3, m->b1, m->c3, m->c1);
    t.c1 = det2x2(m->b1, m->b2, m->c1, m->c2);

    t.a2 = det2x2(m->c2, m->c3, m->a2, m->a3);
    t.b2 = det2x2(m->a1, m->a3, m->c1, m->c3);
    t.c2 = det2x2(m->c1, m->c2, m->a1, m->a2);

    t.a3 = det2x2(m->a2, m->a3, m->b2, m->b3);
    t.b3 = det2x2(m->a3, m->a1, m->b3, m->b1);
    t.c3 = det2x2(m->a1, m->a2, m->b1, m->b2);

    *m = t;

    m3x3_div(m, det);
}

/* m = m + n */
void m3x3_add(struct m3x3_str *m, const struct m3x3_str *n)
{
    m->a1 += n->a1;
    m->a2 += n->a2;
    m->a3 += n->a3;

    m->b1 += n->b1;
    m->b2 += n->b2;
    m->b3 += n->b3;

    m->c1 += n->c1;
    m->c2 += n->c2;
    m->c3 += n->c3;
}

/* m = m - n */
void m3x3_sub(struct m3x3_str *m, const struct m3x3_str *n)
{
    m->a1 -= n->a1;
    m->a2 -= n->a2;
    m->a3 -= n->a3;

    m->b1 -= n->b1;
    m->b2 -= n->b2;
    m->b3 -= n->b3;

    m->c1 -= n->c1;
    m->c2 -= n->c2;
    m->c3 -= n->c3;
}

/* m = s * m */
void m3x3_mul(struct m3x3_str *m, double s)
{
    m->a1 *= s;
    m->a2 *= s;
    m->a3 *= s;
    m->b1 *= s;
    m->b2 *= s;
    m->b3 *= s;
    m->c1 *= s;
    m->c2 *= s;
    m->c3 *= s;
}

/* m = s / m */
void m3x3_div(struct m3x3_str *m, double s)
{
    m->a1 /= s;
    m->a2 /= s;
    m->a3 /= s;
    m->b1 /= s;
    m->b2 /= s;
    m->b3 /= s;
    m->c1 /= s;
    m->c2 /= s;
    m->c3 /= s;
}

/* u = u * m */
void m3x3_vmul(struct v3_str *u, const struct m3x3_str *m)
{
    struct v3_str t;

    t.x = ((u->x * m->a1)
           + (u->y * m->b1)
           + (u->z * m->c1));

    t.y = ((u->x * m->a2)
           + (u->y * m->b2)
           + (u->z * m->c2));

    t.z = ((u->x * m->a3)
           + (u->y * m->b3)
           + (u->z * m->c3));

    *u = t;
}

void m3x3_print(const struct m3x3_str *m)
{
    printf("%f %f %f\n", m->a1, m->a2, m->a3);
    printf("%f %f %f\n", m->b1, m->b2, m->b3);
    printf("%f %f %f\n\n", m->c1, m->c2, m->c3);
}

void m3x3_inv_test(void)
{
    struct m3x3_str m = {1, 2, 3,
                         0, 1, 4,
                         5, 6, 0};
    struct m3x3_str m_inv = {-24, 18, 5,
                             20, -15, -4,
                             -5, 4, 1};

    printf("m:\n");
    m3x3_print(&m);
    m3x3_inv(&m);
    printf("m^{-1}:\n");
    m3x3_print(&m);
    printf("correct:\n");
    m3x3_print(&m_inv);
    printf("inverted back:\n");
    m3x3_inv(&m);
    m3x3_print(&m);
}
