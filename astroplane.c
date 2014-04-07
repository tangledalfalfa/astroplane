#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "ephtime.h"
#include "ephstar.h"
#include "ephutil.h"

#include "coord.h"
#include "vector3.h"

/*
 * gnuplot notes:
 *   set xrange [-442:0] noreverse
 *   set yrange [-135:135]
 *   plot '<file>' using 5:(-$6/$12):($11*0.3/($2<=4)) \
 *         with points linetype 3 pointtype 6 pointsize variable
 *
 *  the "/$12" only plots the completed stars
 *  the "/($2<=4) selects by magnitude
 */

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/* macro to convert degrees, minutes, seconds to decimal degrees */
#define DMS2DEG(d, m, s)                        \
    ((d) >= 0) ?                                \
    ((d) + (((m) + ((s) / 60.0)) / 60.0)) :     \
    ((d) - (((m) + ((s) / 60.0)) / 60.0))

/*
 * Hipparcos Main Catalog
 * heasarc.gsfc.nasa.gov/W3Browse/all/hipparcos.html
 * visual magnitude <= 6.0
 */
#define STARFILE "hip_magle6.dat"
#define POSNFILE "latlon.dat"

#define DFLT_LAT DMS2DEG(44, 35, 26.0)
#define DFLT_LON DMS2DEG(-104, 42, 55.0)

/* time, UTC */
#define PLOTYEAR   2013
#define PLOTMONTH  12
#define PLOTDAY    13
#define PLOTHOUR   14
#define PLOTMINUTE  3
#define PLOTSECOND 22
/* minimum altitude (degrees) */
#define ALT_MIN 5.0
/* room dimensions, cm */
#define OBS_TO_CEIL 152.0       /* observer to ceiling */
#define OBS_TO_WALL  38.0       /* observer to wall */
#define ROOM_NS     270.0       /* north-south dimension */
#define ROOM_EW     442.0       /* east-west dimension */
/* diameter (mm) of 0 magnitude star (vega) at zenith */
#define DIA_0         6.0

/* data parsed from input file */
struct star_rec_str {
    int hip;                     /* Hipparcus catalog number */
    double ra;                   /* right ascension, degrees */
    double dec;                  /* declination, degrees */
    float vmag;                  /* visual magnitude */
};

/*
 * private external variables
 */

/* constants */

static const struct v3_str origin = {0, 0, 0};
/* ceiling origin (south west corner) */
static const struct v3_str p0 = {OBS_TO_WALL - ROOM_EW,
                                 -ROOM_NS / 2.0,
                                 OBS_TO_CEIL};
/* extent of x-axis (north west corner) */
static const struct v3_str px = {OBS_TO_WALL - ROOM_EW,
                                 ROOM_NS / 2.0,
                                 OBS_TO_CEIL};
/* extent of y-axis (south east corner) */
static const struct v3_str py = {OBS_TO_WALL,
                                 -ROOM_NS / 2.0,
                                 OBS_TO_CEIL};

/*
 * private functions
 */

static double ms2deg(int m, float s)
{
    return (m + (s / 60.0)) / 60.0;
}

static double hms2d(int h, int m, float s)
{
    return 15.0 * (h + ms2deg(m, s));
}

/* tricky: degrees could be "-00", so leave as str */
static double dms2d(const char *d, int m, float s)
{
    double sign;
    double ret;

    /* leading sign */
    if (*d == '-') {
        sign = -1;
        d++;
    } else {
        sign = 1;
        if (*d == '+')
            d++;
    }

    /* 0..180 */
    ret = 0;
    while (isdigit(*d)) {
        ret *= 10.0;
        ret += *d++ - '0';
    }

    ret += ms2deg(m, s);

    return ret * sign;
}

static int read_latlon(FILE *in, double *lat, double *lon)
{
    char d[5];
    int m;
    float s;

    if (fscanf(in, "%s %d %f\n", d, &m, &s) == EOF)
        return -1;
    *lat = dms2d(d, m, s);
    if (fscanf(in, "%s %d %f", d, &m, &s) == EOF)
        return -1;
    *lon = dms2d(d, m, s);
    return 0;
}

/* read next star's data from input file, return -1 on EOF */
static int read_star(FILE *in, struct star_rec_str *p)
{
    int ra_hours, ra_minutes;
    float ra_seconds;
    char dec_degrees[4];        /* must be char (-00 case) */
    int dec_minutes;
    float dec_seconds;

    if (fscanf(in, "|HIP %d |%d %d %f|%s %d %f|%f|\n",
               &p->hip,
               &ra_hours, &ra_minutes, &ra_seconds,
               dec_degrees, &dec_minutes, &dec_seconds,
               &p->vmag) == EOF)
        return -1;

#if 0
    printf("|HIP %-6d |%02d %02d %07.4f|%s %02d %06.3f|%5.2f|\n",
           p->hip,
           ra_hours, ra_minutes, ra_seconds,
           dec_degrees, dec_minutes, dec_seconds,
           p->vmag);
#endif

    /* convert to decimal degrees */
    p->ra = hms2d(ra_hours, ra_minutes, ra_seconds);
    p->dec = dms2d(dec_degrees, dec_minutes, dec_seconds);

#if 0
    printf(" %012.8f,%012.8f\n", p->ra, p->dec);
#endif

    return 0;
}


/* M A I N */
int main(int argc, char *argv[])
{
    FILE *starfile;
    FILE *posnfile;
    double lat, lon;
    struct star_rec_str star_rec;
    /* time, UTC */
    struct ymdhms tstar  = {PLOTYEAR, PLOTMONTH, PLOTDAY,
                            PLOTHOUR, PLOTMINUTE, PLOTSECOND};
    struct starData stardat;
    double east, north;

    struct v3_str p0x, p0y;
    /* vector normal to ceiling */
    struct v3_str n;

    /* compute normal to ceiling */
    p0x = px;
    v3_sub(&p0x, &p0);
    p0y = py;
    v3_sub(&p0y, &p0);
    n = p0x;
    v3_cross(&n, &p0y);

    /* read in latitude, longitude */
    posnfile = fopen(POSNFILE, "r");
    if (posnfile != NULL) {
        if (read_latlon(posnfile, &lat, &lon) != 0) {
            lat = DFLT_LAT;
            lon = DFLT_LON;
        }
        fclose(posnfile);
    } else {
        lat = DFLT_LAT;
        lon = DFLT_LON;
    }
#if 1
    printf("lat: %f, lon: %f\n", lat, lon);
#endif

    starfile = fopen(STARFILE, "r");
    while (read_star(starfile, &star_rec) != -1) {
        /* dn is anchored in ne corner, ds in se corner */
        /*
         * dn is wall measurement using NE anchor point
         * ds is wall measurement using SE anchor point
         * N, S walls measured from east side
         * W wall measured from S side _from_both_anchors_
         */
        double dn, ds;
        char wn, ws;    /* wall on which line terminates */
        double bri;     /* brightness of dot (relative to mag 0) */
        double dia;     /* diameter of dot */
        double dist;    /* observer to dot distance */
        /* unit vector in direction of star, spherical coords */
        struct crd_sph_str u_sph;
        struct v3_str u_crt;    /* same in cartesian coords */

        /* calculate altitude, azimuth */
        /*
         * note: these calculations don't quite agree with stellarium's.
         * I suspect it's due to "RA/DE (of date)" calculation
         * (proper motion)
         */
        ephStarPos(&tstar, lat, lon, star_rec.ra, star_rec.dec,
                   &stardat);
#if 0
        //printf("\nStar %d:\n", star_rec.hip);
        //ephStarDump(&stardat);
        printf("%d,%10.6f,%10.6f\n", star_rec.hip, stardat.az, stardat.alt);
#endif

        /* create unit vector in direction of star */
        u_sph.r = 1.0;
        u_sph.phi = ephDegToRad(ephAltToPhi(stardat.alt));
        u_sph.theta = ephDegToRad(ephAzToTheta(stardat.az));
        crd_sph2cart(&u_sph, &u_crt);
        /* distance from origin to dot */
        dist = v3_dist_line_plane(&origin, &u_crt, &p0, &n);

        /* CLEANED UP TO HERE */

        /* skip stars too low (or below horizon) */
        if (stardat.alt < ALT_MIN)
            continue;

        /*
         * convert to cartesian coords:
         *    observer is OBS_TO_CEIL below ceiling
         *    OBS_TO_WALL from middle of east wall
         */
        east = OBS_TO_CEIL * ephSin(stardat.az) / ephTan(stardat.alt);
        north = OBS_TO_CEIL * ephCos(stardat.az) / ephTan(stardat.alt);
        east -= OBS_TO_WALL;
        /* skip those not on ceiling */
        if ((north > ROOM_NS / 2.0) || (north < -ROOM_NS / 2.0)
            || (east > 0) || (east < -ROOM_EW))
            continue;
#if 0
        printf("%d %6.1f %6.1f %5.2f\n", star_rec.hip,
               east, north, star_rec.vmag);
#endif
        if (-east / (ROOM_NS / 2.0 - north) <= ROOM_EW / ROOM_NS) {
            dn = -east * ROOM_NS / (ROOM_NS / 2.0 - north);
            wn = 's';
        } else {
            dn = ROOM_NS - ROOM_EW * (ROOM_NS / 2.0 - north) / -east;
            wn = 'W';
        }

        if (-east / (ROOM_NS / 2.0 + north) <= ROOM_EW / ROOM_NS) {
            ds = -east * ROOM_NS / (ROOM_NS / 2.0 + north);
            ws = 'n';
        } else {
            ds = ROOM_EW * (ROOM_NS / 2.0 + north) / -east;
            ws = 'W';
        }

        /* brightness ratio, relative to mag 0: m = -2.5log_10(F/F0) */
        /* this could be more accurate: 5th root of 100 */
        /* see Wikipedia: apparent magnitude */
        bri = pow(10.0, star_rec.vmag / -2.5);
        /* compensate for distance from observer to dot */
        bri *= dist * dist / (OBS_TO_CEIL * OBS_TO_CEIL);
        /* compensate for view angle */
        bri /= ephSin(stardat.alt);
        /* brightness proportional to square of diameter */
        dia = DIA_0 * sqrt(bri);
#if 1
        printf("%6d %5.2f %010.6f %09.6f %6.1f %6.1f %05.1f %c %05.1f %c %4.1f 0\n",
               star_rec.hip, star_rec.vmag,
               stardat.az, stardat.alt,
               east, north,
               dn, wn, ds, ws, dia);
#endif
    }
    fclose(starfile);
    exit(0);
}
