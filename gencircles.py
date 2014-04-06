#!/usr/bin/env python

# generate LaTeX for range of circles

# Note: these filled circles are incompatible with pdflatex.
#       must save as postscript (.ps) file, and print that.

wbox = 130
hbox = 150

dstep = 0.1
ystep = 10.0

# \circle* is a filled circle
def draw_circle(x, y, diam):
    print '\\put(%s,%s){\\circle*{%s}}' % (x, y, diam)
    print '\\put(%s,%s){%s}' % (x+max(1+diam/2.0,2.0), y-1, diam)

def draw_circles(dstart, dstop, x):
    diam = dstart
    y = hbox - ystep
    while diam <= dstop + dstep / 10.0:
        draw_circle(x, y, diam)
        y -= ystep
        diam += dstep

print '\\documentclass[letterpaper]{article}'
print '\\usepackage{eepic}'
print '\\usepackage{fullpage}'
print '\\setlength{\\unitlength}{1mm}'
print '\\begin{document}'
print '\\begin{picture}(%s,%s)(10,10)' % (wbox, hbox)

dstart = 0.1
xstep = 20
x = 0
for i in xrange(7):
    draw_circles(dstart, dstart + 9 * dstep, x)
    dstart += 10 * dstep
    x += xstep

# special for regulus
draw_circle(xstep * 6, hbox - 12 * ystep, 12.4)

print '\\end{picture}'
print '\\end{document}'
