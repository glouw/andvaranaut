CC = gcc
PROJ = water
CPPFLAGS = -Iinclude
SRCS = main.c Map.c Point.c Display.c Hero.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -g -Ofast -flto -fwhole-program -fassociative-math\
		 -mrecip -freciprocal-math -fno-signed-zeros -fno-trapping-math\
		 -frename-registers -funroll-loops\
		 -Wall -Wpedantic -Wextra -Wshadow -std=gnu99 -pedantic\
		 -Wdouble-promotion -fsingle-precision-constant\
		 -DNDEBUG\
		 #-DPROFILE
LDFLAGS = -lm -lSDL2 -lSDL2_ttf
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

profile:
	startx valgrind ./$(PROJ) maps/test

clean:
	rm -f callgrind.out.*
	rm -f $(PROJ)
	rm -f $(OBJS)
