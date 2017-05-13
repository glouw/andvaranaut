# God's tongue
CC = gcc
# Dialect
CC+= -std=c99
# Project name
PROJ = water
# Source files
SRCS = main.c Play.c Test.c Util.c Point.c Hit.c Line.c Map.c
SRCS+= Sdl.c Display.c Wall.c Hero.c Sprites.c String.c
SRCS+= Portals.c Boundary.c Surfaces.c Light.c Textures.c
SRCS+= Console.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
# Warnings flags
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra
# Debugging flags
CFLAGS+= -g
# Optimization flags
CFLAGS+= -Ofast -flto -fwhole-program -fsingle-precision-constant
CFLAGS+= -fassociative-math -freciprocal-math -fno-signed-zeros
CFLAGS+= -fno-trapping-math -frename-registers -funroll-loops
TARGET_ARCH = -march=native
# Linking flags
LDFLAGS = -lSDL2 -lSDL2_image -lm
# Dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
# Command line definitions
AUX =
# Compile, link, and post process
COMP = $(CC) $(AUX) $(TARGET_ARCH) $(CFLAGS) $(DEPFLAGS) -c
LINK = $(CC) $(AUX) $(TARGET_ARCH) $(CFLAGS) $(OBJS) $(LDFLAGS) -o
POST = mv -f $*.Td $*.d
$(PROJ): $(OBJS)
	$(LINK) $(PROJ)
%.o : %.c
%.o : %.c %.d
	$(COMP) $(OUTPUT_OPTION) $<
	$(POST)
%.d: ;
.PRECIOUS: %.d
-include $(patsubst %,%.d,$(basename $(SRCS)))
# Do as your mother tells you
.PHONY: clean
clean:
	rm -f cachegrind.out.*
	rm -f vgcore.*
	rm -f $(DEPS)
	rm -f $(PROJ)
	rm -f $(OBJS)
