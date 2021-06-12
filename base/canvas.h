#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tuple.h"
#include "../utils/funcs.h"

#pragma once

typedef struct {
    unsigned int width;
    unsigned int height;
    tuple **__canvas;
} Canvas;

Canvas *canvas(unsigned int width, unsigned int height);
void free_canvas(Canvas *c);
void write_pixel(Canvas *c, unsigned int x, unsigned int h, const tuple *color);
tuple *pixel_at(Canvas *c, unsigned int x, unsigned int y);
void canvas_to_ppm(Canvas *c, FILE *stream);

Canvas *canvas(unsigned int width, unsigned int height) {
    Canvas *c = (Canvas*)malloc(sizeof(Canvas));
    c->height = height;
    c->width = width;
    c->__canvas = (tuple**)malloc(sizeof(tuple*) * width * height);
    unsigned int SIZE = width * height;
    for (unsigned int i = 0; i < SIZE; i++) {
        c->__canvas[i] = vec(0,0,0);
    }
    return c;
}

void free_canvas(Canvas *c) {
    unsigned int SIZE = c->width * c->height;
    for (unsigned int i = 0; i < SIZE; i++) {
        free(c->__canvas[i]);
    }
    free(c->__canvas);
    free(c);
}

void write_pixel(Canvas *c, unsigned int x, unsigned int h, const tuple *color) {
    if (x < 0 || x >= c->width) return;
    if (h < 0 || h >= c->height) return;
    tuple * pixel = c->__canvas[x + c->width * h];
    pixel->x = color->x;
    pixel->y = color->y;
    pixel->z = color->z;
}

tuple *pixel_at(Canvas *c, unsigned int x, unsigned int y) {
    return c->__canvas[x + (c->width * y)];
}

void canvas_to_ppm(Canvas *c, FILE *stream) {
    // PPM header
    const int MAX_LINE_WIDTH = 70;
    const unsigned int MAX_CHUNK_SIZE = 12;
    fprintf(stream, "P3\n%d %d\n%d", c->width, c->height, 255);
    unsigned int SIZE = c->width * c->height;
    int chars_written = 0;
    for (unsigned int i = 0; i < SIZE; i++) {
        if (i % c->width == 0 || MAX_CHUNK_SIZE - chars_written - MAX_CHUNK_SIZE <= 0) {
            chars_written = 0;
            fprintf(stream, "\n");
        }
        tuple *pixel = pixel_at(c, i % c->width, i / c->width);
        int x = CLAMP(0, (int)(pixel->x * 255), 255);
        int y = CLAMP(0, (int)(pixel->y * 255), 255);
        int z = CLAMP(0, (int)(pixel->z * 255), 255);
        chars_written += fprintf(stream, "%d %d %d ", x, y, z);
    }
    fprintf(stream, "\n");
}
