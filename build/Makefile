
CFLAGS 	+= -g -Wall -Wextra -Wpedantic

CFLAGS  += -I../include
LDLIBS  += -ldl

CFLAGS 	+= $(shell pkg-config --cflags gl)
LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other gl)
LDLIBS 	+= $(shell pkg-config --libs-only-l gl)

CFLAGS 	+= $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other sdl2)
LDLIBS 	+= $(shell pkg-config --libs-only-l sdl2)

vpath %.h 	../include ../src
vpath %.c 	../src

OBJS = glad.o shader.o window.o draw.o

all: main

main: window.h draw.h $(OBJS)

draw.o: draw.h shader.h

clean:
	$(RM) *.o
