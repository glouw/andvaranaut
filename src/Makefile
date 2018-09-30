# -----------------------------------------------------------------------------
#
#                            A N D V A R A N A U T
#                          ------------------------
#                             THE DUNGEON CRAWLER
#
# -----------------------------------------------------------------------------
# SDL2-devel
# SDL2-devel-ttf

CC  = gcc -std=c99
CXX = g++ -std=c++98
CMP = $(CC)# Use the C++ compiler for extra type checking.

NAME = ../andvaranaut

BIN = $(NAME)

SRCS  = main.c
SRCS += util.c
SRCS += Args.c
SRCS += Atom.c
SRCS += Attack.c
SRCS += Bundle.c
SRCS += Classification.c
SRCS += Clamped.c
SRCS += Compass.c
SRCS += Flow.c
SRCS += Font.c
SRCS += Field.c
SRCS += Gauge.c
SRCS += Hero.c
SRCS += Hits.c
SRCS += Input.c
SRCS += Item.c
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
SRCS += Sprites.c
SRCS += Surfaces.c
SRCS += Textures.c
SRCS += Torch.c
SRCS += Tris.c
SRCS += Vram.c
SRCS += World.c

CMP_FLAGS  = -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion -Wunused-result
CMP_FLAGS += -g -fsanitize=address#Runtime address and bounds checker(Disable for faster builds and performance)
CMP_FLAGS += -Ofast -march=native -pipe
CMP_FLAGS += -flto

LIB_FLAGS  = -lm
LIB_FLAGS += -lSDL2
LIB_FLAGS += -lSDL2_ttf

$(BIN): $(SRCS:.c=.o)
	@echo LINK $(BIN)
	@$(CMP) $(CMP_FLAGS) $(SRCS:.c=.o) $(LIB_FLAGS) -o $(BIN)

%.o : %.c Makefile# Changing the Makefile will recompile everything.
	@echo COMPILE $*.c
	@$(CMP) $(CMP_FLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	@rm -f $*.d
	@mv -f $*.td $*.d

%.d: ;
-include *.d

clean:
	rm -f vgcore.*
	rm -f cachegrind.out.*
	rm -f callgrind.out.*
	rm -f $(BIN)
	rm -f $(SRCS:.c=.o)
	rm -f $(SRCS:.c=.d)
