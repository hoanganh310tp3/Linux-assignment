CC = gcc
CFLAGS = -Wall -O2

SRCDIR = .
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
BIN = $(patsubst $(SRCDIR)/%.c,%,$(SRC))

all: $(BIN)

%: $(OBJDIR)/%.o
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(BIN)