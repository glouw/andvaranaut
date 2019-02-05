# -----------------------------------------------------------------------------
#
#                            A N D V A R A N A U T
#                          ------------------------
#                             THE DUNGEON CRAWLER
#
# -----------------------------------------------------------------------------
#
# Dependencies:
#   SDL2-devel.
#   SDL2-devel-ttf.
#

BIN = ../andvaranaut

#
# 0: Fastest performance.
# 1: Best crash messages.
#

DEBUG = 0

#
# 0: Compile with gcc.
# 1: Compile with clang.
#

CLANG = 0

#
# 0: Compile as C.
# 1: Compile as C++ (Better type checking).
#

CPP = 0

SRCS  = main.c
SRCS += util.c
SRCS += Args.c
SRCS += Atom.c
SRCS += Bundle.c
SRCS += Classification.c
SRCS += Clamped.c
SRCS += Compass.c
SRCS += Embers.c
SRCS += Fire.c
SRCS += Flow.c
SRCS += Font.c
SRCS += Field.c
SRCS += Gauge.c
SRCS += Hero.c
SRCS += Hits.c
SRCS += Input.c
SRCS += Item.c
SRCS += Identification.c
SRCS += Items.c
SRCS += Inventory.c
SRCS += Line.c
SRCS += Map.c
SRCS += Overview.c
SRCS += Point.c
SRCS += Points.c
SRCS += Projection.c
SRCS += Rooms.c
SRCS += Sdl.c
SRCS += Speech.c
SRCS += State.c
SRCS += Title.c
SRCS += Text.c
SRCS += Theme.c
SRCS += Timer.c
SRCS += Scanline.c
SRCS += Sprite.c
SRCS += Scroll.c
SRCS += Sorter.c
SRCS += Sprites.c
SRCS += Surfaces.c
SRCS += Textures.c
SRCS += Torch.c
SRCS += Tris.c
SRCS += Vram.c
SRCS += World.c

ifeq (00, $(CLANG)$(CPP))
COMPILER = gcc -std=c99
endif

ifeq (01, $(CLANG)$(CPP))
COMPILER = g++ -std=c++98
endif

ifeq (10, $(CLANG)$(CPP))
COMPILER = clang -std=c99
endif

ifeq (11, $(CLANG)$(CPP))
COMPILER = clang++ -std=c++98
endif

FLAGS = -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion -Ofast -march=native

ifeq (1, $(DEBUG))
FLAGS += -g -fsanitize=address
else
FLAGS += -flto
endif

LIBS = -lm -lSDL2 -lSDL2_ttf

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

$(BIN): $(OBJS) $(DEPS)
	$(COMPILER) $(FLAGS) $(OBJS) $(LIBS) -o $(BIN)

%.o : %.c %.d Makefile
	$(COMPILER) $(FLAGS) -MMD -MP -MT $@ -MF $*.d -c $<

-include *.d

%.d : ;

clean:
	rm -f $(BIN)
	rm -f $(OBJS)
	rm -f $(DEPS)
