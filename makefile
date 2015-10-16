all:
	gcc main.c objects.c types.c system.c expressions.c garbage.c $(shell find clib -name '*.c') -o zapper
