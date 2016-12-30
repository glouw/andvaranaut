CC = gcc
PROJ = water
CPPFLAGS = -Iinclude
SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Wshadow -std=c11 -pedantic -Ofast
LDFLAGS = -lm -lSDL2
TARGET_ARCH = -march=native
DEPDIR := deps
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK = $(CC) $(OBJS) $(TARGET_ARCH) $(LDFLAGS) -o
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
