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
	$(RM) *.o $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

astroplane.o: ephtime.h ephstar.h ephutil.h coord.h vector3.h
coord.o: coord.h vector3.h
ephstar.o: ephstar.h ephtime.h ephutil.h
ephtime.o: ephtime.h ephutil.h
ephutil.o: ephutil.h
matrix3x3.o: matrix3x3.h vector3.h
vector3.o: vector3.h
