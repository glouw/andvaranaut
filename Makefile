CC = gcc -std=c99
PROJ = water
SRCS = main.c Play.c Test.c Util.c Point.c Hit.c Line.c Map.c
SRCS+= Sdl.c Display.c Wall.c Hero.c Sprites.c String.c
SRCS+= Portals.c Boundary.c Surfaces.c Light.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra -Werror
CFLAGS+= -g
CFLAGS+= -Ofast -flto -fwhole-program -fsingle-precision-constant
CFLAGS+= -fassociative-math -freciprocal-math -fno-signed-zeros
CFLAGS+= -fno-trapping-math -frename-registers -funroll-loops
LDFLAGS = -lSDL2 -lSDL2_image -lm
TARGET_ARCH = -march=native
DEPDIR := dep
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMP = $(CC) $(AUX) $(TARGET_ARCH) $(CFLAGS) $(DEPFLAGS) -c
LINK = $(CC) $(AUX) $(TARGET_ARCH) $(CFLAGS) $(OBJS) $(LDFLAGS) -o
POST = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(PROJ): $(OBJS)
	$(LINK) $(PROJ)

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMP) $(OUTPUT_OPTION) $<
	$(POST)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

love:
	@echo "not war?"

clean:
	rm -f cachegrind.out.*
	rm -f vgcore.*
	rm -f $(DEPDIR)/*.d
	rm -f $(PROJ)
	rm -f $(OBJS)
