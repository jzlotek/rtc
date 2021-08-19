#include "../rtc.h"

Canvas *canvas(unsigned int width, unsigned int height) {
    Canvas *c = (Canvas*)malloc(sizeof(Canvas));
    c->height = height;
    c->width = width;
    c->__canvas = (Tuple**)malloc(sizeof(Tuple*) * width * height);
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

void write_pixel(Canvas *c, unsigned int x, unsigned int h, const Tuple *color) {
    if (x < 0 || x >= c->width) return;
    if (h < 0 || h >= c->height) return;
    Tuple * pixel = c->__canvas[x + c->width * h];
    pixel->vals[0] = color->vals[0];
    pixel->vals[1] = color->vals[1];
    pixel->vals[2] = color->vals[2];
}

Tuple *pixel_at(Canvas *c, unsigned int x, unsigned int y) {
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
        Tuple *pixel = pixel_at(c, i % c->width, i / c->width);
        int x = CLAMP(0, (int)(pixel->vals[0] * 255), 255);
        int y = CLAMP(0, (int)(pixel->vals[1] * 255), 255);
        int z = CLAMP(0, (int)(pixel->vals[2] * 255), 255);
        chars_written += fprintf(stream, "%d %d %d ", x, y, z);
    }
    fprintf(stream, "\n");
}
