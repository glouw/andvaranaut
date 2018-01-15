# Compiler and standard.
CC = gcc -std=c99

# Project name.
PROJ = andvaranaut

# Source files.
SRCS = main.c
SRCS+= util.c
SRCS+= Args.c
SRCS+= Bundle.c
SRCS+= Current.c
SRCS+= Compass.c
SRCS+= Display.c
SRCS+= Field.c
SRCS+= Found.c
SRCS+= Gauge.c
SRCS+= Hero.c
SRCS+= Hits.c
SRCS+= Input.c
SRCS+= Line.c
SRCS+= Map.c
SRCS+= Overview.c
SRCS+= Point.c
SRCS+= Projection.c
SRCS+= State.c
SRCS+= Sdl.c
SRCS+= Scanline.c
SRCS+= Sprites.c
SRCS+= Surfaces.c
SRCS+= Textures.c
SRCS+= Torch.c
SRCS+= Weapon.c

# Warnings flags.
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra -Wimplicit-fallthrough=5

# Debugging flags.
CFLAGS+= -g

# Optimization flags.
CFLAGS+= -Ofast -march=native -flto -fsingle-precision-constant
CFLAGS+= -fassociative-math -freciprocal-math -fno-signed-zeros
CFLAGS+= -frename-registers -funroll-loops -fno-trapping-math

# Linker flags.
LDFLAGS = -lm -lSDL2

# Linker.
$(PROJ): $(SRCS:.c=.o)
	$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(PROJ)

# Compiler template; generates dependency targets.
%.o : %.c
	$(CC) $(CFLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	@mv -f $*.td $*.d

# All dependency targets.
%.d: ;
-include *.d

clean:
	rm -f vgcore.*
	rm -f cachegrind.out.*
	rm -f callgrind.out.*
	rm -f $(PROJ)
	rm -f $(SRCS:.c=.o)
	rm -f $(SRCS:.c=.d)
