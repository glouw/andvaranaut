CC = gcc
PROJ = water
SRCS = main.c util.c Point.c Hit.c Line.c Map.c Gpu.c Display.c Wall.c Hero.c\
	   Portals.c Frame.c Blocks.c Meta.c Surfaces.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -std=gnu99 -g -Ofast -flto -fwhole-program -Wall -Wpedantic -Wextra -Wshadow
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
