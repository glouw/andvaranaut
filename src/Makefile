# -----------------------------------------------------------------------------
#
#                            A N D V A R A N A U T
#                          ------------------------
#                             THE DUNGEON CRAWLER
#
#                            Makefile for use with
#                            Linux, MacOS, Windows
#
# -----------------------------------------------------------------------------
# NOTE:
# ComSpec is referenced for building on Windows with MINGW32.
#
# Required libraries
#
# 	- SDL2-devel
#
# -> Linux
# Use your package manager to get the required libraries
# and then run make.
#
# -> MacOS
# Use brew as your package manager, get the required libraries,
# and then run make.
#
# -> Windows
# Get the required MINGW32 developement libraries from the SDL2 website
# and place them two folders up from the Makefile and run mingw32-make
# and then copy over the SDL2 .dll files to where the andvaranaut
# executable lies.
#
# For a quicker compile, run:
#     make -j10

# -----------------------------------------------------------------------------
# Compiler.
# -----------------------------------------------------------------------------
# The entirety of the Andvaranaut source compiles cleanly with a C++ compiler.
# This extra effort was maintained in the hopes that this project one day grows
# with the helping hand of the open-source world.

CC = gcc

# -----------------------------------------------------------------------------
# Binary Name.
# -----------------------------------------------------------------------------

NAME = ../andvaranaut
ifdef ComSpec
	BIN = $(NAME).exe
else
	BIN = $(NAME)
endif

# -----------------------------------------------------------------------------
# Source Files.
# -----------------------------------------------------------------------------

SRCS =
SRCS += main.c
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
SRCS += Hit.c
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
SRCS += Ray.c
SRCS += Rooms.c
SRCS += Sdl.c
SRCS += Title.c
SRCS += Theme.c
SRCS += Timer.c
SRCS += Sheer.c
SRCS += Scanline.c
SRCS += Sprite.c
SRCS += Scroll.c
SRCS += Sprites.c
SRCS += Surfaces.c
SRCS += Textures.c
SRCS += Torch.c
SRCS += Tris.c
SRCS += World.c

# -----------------------------------------------------------------------------
# Compiler Flags.
# -----------------------------------------------------------------------------

CFLAGS =
ifdef ComSpec
	CFLAGS += -I../../SDL2-2.0.7/i686-w64-mingw32/include
endif

# Standard.
CFLAGS += -std=c99

# Warnings.
CFLAGS += -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion -Wunused-result

# Debugging.
CFLAGS += -g

# General optimizations.
CFLAGS += -Ofast -march=native -pipe

# Link time optimizations.
CFLAGS += -flto

# -----------------------------------------------------------------------------
# Library Flags.
# -----------------------------------------------------------------------------

LDFLAGS =
ifdef ComSpec
	LDFLAGS += -L../../SDL2-2.0.7/i686-w64-mingw32/lib
	LDFLAGS += -lmingw32
	LDFLAGS += -lSDL2main
endif
LDFLAGS += -lm
LDFLAGS += -lSDL2
LDFLAGS += -lSDL2_ttf

# -----------------------------------------------------------------------------
# Linker.
# -----------------------------------------------------------------------------

ifdef ComSpec
	RM = del /F /Q
	MV = ren
else
	RM = rm -f
	MV = mv -f
endif

$(BIN): $(SRCS:.c=.o)
	@echo LINK $(BIN)
	@$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(BIN)
	@echo "If you are on Windows be sure to copy over all SDL2 .dll"
	@echo "files to where andvaranaut.exe was placed."

# -----------------------------------------------------------------------------
# Compiler.
# -----------------------------------------------------------------------------
# Dependency files (.d) are generated during build to keep track of
# which .c files were affected by .h updates.

%.o : %.c Makefile
	@echo CC $*.c
	@$(CC) $(CFLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	@$(RM) $*.d
	@$(MV) $*.td $*.d
%.d: ;
-include *.d

# -----------------------------------------------------------------------------
# Cleaner.
# -----------------------------------------------------------------------------

clean:
	$(RM) vgcore.*
	$(RM) cachegrind.out.*
	$(RM) callgrind.out.*
	$(RM) $(BIN)
	$(RM) $(SRCS:.c=.o)
	$(RM) $(SRCS:.c=.d)

# -----------------------------------------------------------------------------
# You actually read the Makefile?
# -----------------------------------------------------------------------------

love:
	@echo "not war?"
