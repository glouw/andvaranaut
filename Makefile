CC = gcc -std=c99
PROJ = water
SRCS = main.c test.c util.c Point.c Hit.c Line.c Map.c Gpu.c Display.c Wall.c Hero.c
SRCS+= Portals.c Scanline.c Blocks.c Surfaces.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra
CFLAGS+= -g
CFLAGS+= -Ofast -flto -fwhole-program -fsingle-precision-constant -fassociative-math
CFLAGS+= -freciprocal-math -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops
LDFLAGS = -lSDL2 -lm
TARGET_ARCH = -march=native
DEPDIR := dep
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMP = $(CC) $(TARGET_ARCH) $(CFLAGS) $(DEPFLAGS) -c
LINK = $(CC) $(TARGET_ARCH) $(CFLAGS) $(OBJS) $(LDFLAGS) -o
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
