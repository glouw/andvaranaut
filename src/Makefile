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
# 	- SDL2-ttf
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
# and then copy over the SDL2.dll files to where the andvaranaut
# executable lies.
#
# For a quicker compile, run:
#     make -j10

# -----------------------------------------------------------------------------
# Compiler.
# -----------------------------------------------------------------------------
CC  = gcc -std=c99
CXX = g++ -std=c++98

# Although Andvaranaut was written in C, the entirety of its source compiles
# cleanly with a C++ compiler.
# You may select your compiler here (either CC or CXX defined above).
CMP = $(CC)

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
SRCS += Ttf.c
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

# -----------------------------------------------------------------------------
# Compiler Flags.
# -----------------------------------------------------------------------------

CMP_FLAGS =

ifdef ComSpec
CMP_FLAGS += -I../../SDL2*/i686-w64-mingw32/include
endif

# Warnings.
CMP_FLAGS += -Wshadow -Wall -Wpedantic -Wextra -Wdouble-promotion -Wunused-result 
CMP_FLAGS += -Wstrict-prototypes

# Debugging.
CMP_FLAGS += -g

# General optimizations.
CMP_FLAGS += -Ofast -march=native -pipe

# Link time optimizations.
CMP_FLAGS += -flto

# -----------------------------------------------------------------------------
# Library Flags.
# -----------------------------------------------------------------------------

LIB_FLAGS =

ifdef ComSpec
LIB_FLAGS += -L../../SDL2*/i686-w64-mingw32/lib
LIB_FLAGS += -lmingw32
LIB_FLAGS += -lSDL2main
endif

LIB_FLAGS += -lm
LIB_FLAGS += -lSDL2
LIB_FLAGS += -lSDL2_ttf

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
	@$(CMP) $(CMP_FLAGS) $(SRCS:.c=.o) $(LIB_FLAGS) -o $(BIN)
	@echo "If you are on Windows be sure to copy over all SDL2.dll and SDL2-ttf.dll"
	@echo "files to where andvaranaut.exe was placed."

# -----------------------------------------------------------------------------
# Compiler.
# -----------------------------------------------------------------------------
# Dependency files (.d) are generated during build to keep track of
# which .c files were affected by .h updates.

%.o : %.c Makefile
	@echo COMPILE $*.c
	@$(CMP) $(CMP_FLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
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
