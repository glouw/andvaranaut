# Makefile for use with Windows and Linux.
#
# Windows:
# 		mingw32-make
# Unix:
# 		make

CC = gcc

NAME = ../andvaranaut

SRCS =
SRCS+= main.c
SRCS+= util.c
SRCS+= Args.c
SRCS+= Atom.c
SRCS+= Attack.c
SRCS+= Bundle.c
SRCS+= Classification.c
SRCS+= Clamped.c
SRCS+= Flow.c
SRCS+= Compass.c
SRCS+= Field.c
SRCS+= Gauge.c
SRCS+= Hero.c
SRCS+= Hit.c
SRCS+= Hits.c
SRCS+= Input.c
SRCS+= Item.c
SRCS+= Items.c
SRCS+= Inventory.c
SRCS+= Line.c
SRCS+= Map.c
SRCS+= Overview.c
SRCS+= Point.c
SRCS+= Points.c
SRCS+= Projection.c
SRCS+= Ray.c
SRCS+= State.c
SRCS+= Sdl.c
SRCS+= Sheer.c
SRCS+= Scanline.c
SRCS+= Sprite.c
SRCS+= Sprites.c
SRCS+= Surfaces.c
SRCS+= Textures.c
SRCS+= Torch.c
SRCS+= Tris.c
SRCS+= Tinn.c
SRCS+= World.c

# CompSpec defined in windows environment.
ifdef ComSpec
	BIN = $(NAME).exe
else
	BIN = $(NAME)
endif

CFLAGS =
ifdef ComSpec
	CFLAGS += -I ../SDL2-2.0.7/i686-w64-mingw32/include
endif
CFLAGS += -std=c99
CFLAGS += -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion -Wunused-result
CFLAGS += -g
CFLAGS += -Ofast -march=native -pipe
CFLAGS += -flto

LDFLAGS =
ifdef ComSpec
	LDFLAGS += -L..\SDL2-2.0.7\i686-w64-mingw32\lib
	LDFLAGS += -lmingw32
	LDFLAGS += -lSDL2main
endif
LDFLAGS += -lSDL2 -lm

ifdef ComSpec
	RM = del /F /Q
	MV = ren
else
	RM = rm -f
	MV = mv -f
endif

# Link.
$(BIN): $(SRCS:.c=.o)
	@echo $(CC) *.o -o $(BIN)
	@$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(BIN)

# Compile.
%.o : %.c Makefile
	@echo $(CC) -c $*.c
	@$(CC) $(CFLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	@$(RM) $*.d
	@$(MV) $*.td $*.d
%.d: ;
-include *.d

clean:
	$(RM) vgcore.*
	$(RM) cachegrind.out.*
	$(RM) callgrind.out.*
	$(RM) $(BIN)
	$(RM) $(SRCS:.c=.o)
	$(RM) $(SRCS:.c=.d)
