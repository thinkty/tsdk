.POSIX:    # Parse it an run in POSIX conforming mode
.SUFFIXES: # Delete the default suffixes (inference rules)

CC=gcc
CFLAGS=-g -Wall -I$(IDIR)
LDLIBS=-lncurses
OUTPUT=sudoku
ROOTDIR=.
IDIR =$(ROOTDIR)/include
SDIR =$(ROOTDIR)/src
ODIR =$(ROOTDIR)/obj

_DEPS = sudoku.h solve.h tui.h levels.h common.h
DEPS = $(addprefix $(IDIR)/,$(_DEPS))

_OBJS = main.o sudoku.o solve.o tui.o levels.o common.o
OBJS = $(addprefix $(ODIR)/,$(_OBJS))

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJS): | $(ODIR)

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean bear
clean:
	rm -rf $(ODIR) $(OUTPUT)

bear: clean
	bear -- make

