.PHONY: test cannon_ppm clock

CC=gcc
CFLAGS=-O2 -g -lm

%.o: **/%.{c,h}
	$(CC) -c $*.c **/*.c $(CFLAGS)

test: tests_main.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@

cannon: demos/cannon_ppm.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ > cannon.ppm

clock: demos/clock.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ > clock.ppm
