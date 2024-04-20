# Compiler
CC = gcc

# Common flags
CFLAGS = -Wall -Wextra

# Debug flags
DBGFLAGS = -ggdb -O0 -fsanitize=address -fsanitize=undefined

# Release flags
RFLAGS = -O3 -march=native

# Source files
SRCS = main.c hashmap.c vector.c halloc.c 

# Output directories
OUTDIR = target
DBGDIR = $(OUTDIR)/debug
RELDIR = $(OUTDIR)/release

# Object files
OBJS = $(SRCS:.c=.o)


# Debug target
DEBUG = $(DBGDIR)/halloc-debug

# Release target
RELEASE = $(RELDIR)/halloc-release

# Create output directories
$(shell mkdir -p $(DBGDIR) $(RELDIR))

# Debug build
$(DEBUG): CFLAGS += $(DBGFLAGS)
$(DEBUG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Release build
$(RELEASE): CFLAGS += $(RFLAGS)
$(RELEASE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target (remove object files, executables, and output directories)
clean:
	rm -rf $(OBJS) $(DEBUG) $(RELEASE) $(OUTDIR)

# Phony targets
.PHONY: clean
