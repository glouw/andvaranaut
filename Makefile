CC = clang -std=c99
# Project name
PROJ = water
# Source files
SRCS = main.c Play.c Test.c Util.c Point.c Hit.c Line.c Map.c
SRCS+= Sdl.c Display.c Wall.c Hero.c Sprites.c String.c
SRCS+= Sliver.c Surfaces.c Torch.c Textures.c Keys.c
SRCS+= Gui.c Input.c Attack.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
# Warnings flags
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra -Wpadded
# Debugging flags
CFLAGS+= -g
# Optimization flags
CFLAGS+= -Ofast -flto
ARCHITECTURE = -march=native
# Linking flags
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
# Dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
# Compile, link, and post process
COMP = $(CC) $(ARCHITECTURE) $(CFLAGS) $(DEPFLAGS) -c
LINK = $(CC) $(ARCHITECTURE) $(CFLAGS) $(OBJS) $(LDFLAGS) -o
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

.PHONY: headers
headers:
	python headers.py

.PHONY: clean
clean:
	rm -f cachegrind.out.*
	rm -f vgcore.*
	rm -f $(DEPS)
	rm -f $(PROJ)
	rm -f $(OBJS)
