# Compiler and standard
CC = gcc -std=c99

# Project name
PROJ = water

# Source files
SRCS = main.c
SRCS+= util.c
SRCS+= Args.c
SRCS+= Compass.c
SRCS+= Field.c
SRCS+= Display.c
SRCS+= Hero.c
SRCS+= Hit.c
SRCS+= Input.c
SRCS+= Line.c
SRCS+= Map.c
SRCS+= Point.c
SRCS+= Sdl.c
SRCS+= Scanline.c
SRCS+= Sprites.c
SRCS+= Surfaces.c
SRCS+= Textures.c
SRCS+= Torch.c
SRCS+= Projection.c

# Warnings flags
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra -Wpadded -isystem lib

# Debugging flags
CFLAGS+= -g

# Optimization flags
CFLAGS+= -Ofast -flto -march=native

# Linker flags
LDFLAGS = -lm -lSDL2

# Linker
$(PROJ): $(SRCS:.c=.o)
	$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(PROJ)

# Compiler template; generates dependency targets
%.o : %.c
	$(CC) $(CFLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	@mv -f $*.td $*.d

# All dependency targets
%.d: ;
-include *.d

clean:
	rm -f vgcore.*
	rm -f cachegrind.out.*
	rm -f callgrind.out.*
	rm -f $(PROJ)
	rm -f $(SRCS:.c=.o)
	rm -f $(SRCS:.c=.d)
