unexport LC_ALL
LC_COLLATE=C
LC_NUMERIC=C
export LC_COLLATE LC_NUMERIC

VERSION	= 0
PATCHLEVEL	= 0
SUBLEVEL	= 0
NAME	= gwuslab of Linux

MAKE	:= make
NASM	:= nasm
CC	:= gcc
LD	:= ld

TARGET	:= main
SRC	+= malloc.c main.c kmem_cache.c hash.c
OBJ	+= $(SRC:.c=.o)
INCLUDE_PATH	+= .
LIBRARY_PATH	+= /lib
MAKEFLAGS	+= -rR --no-print-directory
CFLAGS	+= $(foreach dir, $(INCLUDE_PATH), -I$(dir))
LDFLAGS	+= $(foreach lib, $(LIBRARY_PATH), -L$(lib))

CFLAGS += -g

export VERSION PATCHLEVEL SUBLEVEL NAME 
export MAKE NASM CC LD CPP 
export CFLAGS LDFLAGS MAKEGLAGS

PHONY += all
PHONY += build
PHONY += clean
.PHONY : $(PHONY)

all:
	$(CC) $(CFLAGS) -o main $(SRC)

buld:

clean:


#kmem_cache_node.o : Kmem_cache.h
#slab.o kmem_cache.o: slab.h
