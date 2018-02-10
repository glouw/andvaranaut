CC = gcc

NAME = andvaranaut

SRCS =
SRCS+= main.c
SRCS+= util.c
SRCS+= Args.c
SRCS+= Attack.c
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
SRCS+= Trapdoor.c
SRCS+= Torch.c
SRCS+= Tris.c

# CompSpec defined in windows environment.
ifdef ComSpec
	BIN = $(NAME).exe
else
	BIN = $(NAME)
endif

CFLAGS =
ifdef ComSpec
	CFLAGS += -I ../SDL2-2.0.7/i686-w64-mingw32/include
	CFLAGS += -I ../SDL2-2.0.7/i686-w64-mingw32/include/SDL2
endif
CFLAGS += -std=c99
CFLAGS += -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion
CFLAGS += -g
CFLAGS += -Ofast -march=native
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
	@echo CC *.o -o $(BIN)
	@$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(BIN)

# Compile.
%.o : %.c
	@echo CC $*.c
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
