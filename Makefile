.PHONY: build clean test cannon clock sphere phong render plane plane_stripe

CC:=gcc
CFLAGS:=-O2 -g -lm -v
# -march=armv8-a+fp+simd+crc -v
SRC:=./rtc
OBJ:=./build
SRCS:=$(wildcard $(SRC)/*.c)
OBJS:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

build:
	mkdir -p build

clean:
	rm -f build/*

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

test: tests/tests_main.c $(OBJS)
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

plane_stripe: demos/plane_striped.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@ | pnmtopng > plane_stripe.png
