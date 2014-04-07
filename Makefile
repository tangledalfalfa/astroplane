CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I.
LIBS = -lm
SRCS =  astroplane.c coord.c ephstar.c ephtime.c ephutil.c \
	matrix3x3.c vector3.c
OBJS = $(SRCS:.c=.o)
MAIN = astroplane

.PHONY: depend clean

all:     $(MAIN)
	@echo compiled

$(MAIN): $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN) TAGS

depend: $(SRCS)
	makedepend $(INCLUDES) $^

TAGS: $(SRCS)
	$(RM) TAGS
	etags $(SRCS)

# DO NOT DELETE

astroplane.o: /usr/include/stdlib.h /usr/include/features.h
astroplane.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
astroplane.o: /usr/include/_G_config.h /usr/include/wchar.h
astroplane.o: /usr/include/xlocale.h /usr/include/math.h ephtime.h ephstar.h
astroplane.o: ephutil.h coord.h vector3.h
coord.o: /usr/include/math.h /usr/include/features.h coord.h vector3.h
ephstar.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
ephstar.o: /usr/include/_G_config.h /usr/include/wchar.h
ephstar.o: /usr/include/xlocale.h /usr/include/math.h ephstar.h ephtime.h
ephstar.o: ephutil.h
ephtime.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
ephtime.o: /usr/include/_G_config.h /usr/include/wchar.h
ephtime.o: /usr/include/xlocale.h ephtime.h ephutil.h
ephutil.o: /usr/include/math.h /usr/include/features.h ephutil.h
matrix3x3.o: /usr/include/stdio.h /usr/include/features.h
matrix3x3.o: /usr/include/libio.h /usr/include/_G_config.h
matrix3x3.o: /usr/include/wchar.h /usr/include/xlocale.h /usr/include/math.h
matrix3x3.o: matrix3x3.h vector3.h
vector3.o: /usr/include/math.h /usr/include/features.h vector3.h
