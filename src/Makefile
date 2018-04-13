# -----------------------------------------------------------------------------
# Makefile or use with:
# 		Linux, MacOS, Windows
# -----------------------------------------------------------------------------
# ComSpec is referenced in Windows for building on Windows.
#
# Windows:
#
# 		Get all relevent SDL2 development libraries from the
# 		SDL2 website and place them two folders up from this makefile.
# 		Run:
#
# 			mingw32-make
#
# 		Once the binary is built copy over all relevant SDL2 .dll files 
# 		to where the final binary is.
#
# Linux:
# 		Use your package manager to get all relevant SDL2 development
# 		libraries and run:
#
# 			make
#
# MacOS:
# 		Use brew to get all relevant SDL2 development libraries and run:
#
# 			make

# -----------------------------------------------------------------------------
# Compiler.
# -----------------------------------------------------------------------------
CC = gcc

# -----------------------------------------------------------------------------
# Binary Name.
# -----------------------------------------------------------------------------
NAME = ../andvaranaut
# Windows requires binary to have .exe postfix.
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
SRCS += Flow.c
SRCS += Compass.c
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
SRCS += State.c
SRCS += Sdl.c
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
# Post Build Linking.
# -----------------------------------------------------------------------------
LDFLAGS =
ifdef ComSpec
	LDFLAGS += -L../../SDL2-2.0.7/i686-w64-mingw32/lib
	LDFLAGS += -lmingw32
	LDFLAGS += -lSDL2main
endif
LDFLAGS += -lm
LDFLAGS += -lSDL2

# -----------------------------------------------------------------------------
# Build Linker.
# -----------------------------------------------------------------------------
ifdef ComSpec
	RM = del /F /Q
	MV = ren
else
	RM = rm -f
	MV = mv -f
endif
$(BIN): $(SRCS:.c=.o)
	@echo $(CC) *.o -o $(BIN)
	@$(CC) $(CFLAGS) $(SRCS:.c=.o) $(LDFLAGS) -o $(BIN)

# -----------------------------------------------------------------------------
# Build Compiler.
# -----------------------------------------------------------------------------
# Dependency files (.d) are generated during build to keep track of
# which .c files were affected by .h updates.
%.o : %.c Makefile
	@echo $(CC) -c $*.c
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

love:
	@echo "not war?"
