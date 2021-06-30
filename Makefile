.PHONY: test cannon clock sphere phong render plane

CC=gcc
CFLAGS=-O2 -g -lm

%.o: **/%.{c,h}
	$(CC) -c $*.c **/*.c $(CFLAGS)

test: tests/tests_main.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@

cannon: demos/cannon_ppm.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > cannon.png

clock: demos/clock.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > clock.png

sphere: demos/sphere.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > sphere.png

phong: demos/phong.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > phong.png

render: demos/render.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > render.png

plane: demos/plane.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > plane.png
