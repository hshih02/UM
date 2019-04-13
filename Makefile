#
# Makefile for the UM Test lab
# 
CC = gcc

IFLAGS  = -I/comp/40/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/lib64 -L/usr/sup/cii40/lib64
LDLIBS  = -lum-dis -lcii -lbitpack -l40locality -lcii40 -lm

INCLUDES = $(shell echo *.h)

EXECS   = um

all: $(EXECS)

um: um.o process.o seg_mem.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o

