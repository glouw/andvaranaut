CC = gcc
PROJ = water
CPPFLAGS = -Iinclude
SRCS = main.c map.c geom.c precalc.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Wshadow -std=c99 -pedantic -Ofast -flto
LDFLAGS = -lm -lSDL2
TARGET_ARCH = -march=native
DEPDIR := deps
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE = $(CC) $(CFLAGS) $(DEPFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK = $(CC) $(CFLAGS) $(OBJS) $(TARGET_ARCH) $(LDFLAGS) -o
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(PROJ): $(OBJS)
	$(LINK) $(PROJ)

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE) $(OUTPUT_OPTION) $<
	@$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
