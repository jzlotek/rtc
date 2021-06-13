#include "../base/canvas.h"
#include "../utils/consts.h"
#include "test.h"

#pragma once


void TestCanvasCreation() {
    Canvas *c = canvas(10, 20);
    Equal(c->width, 10);
    Equal(c->height, 20);
    Tuple color = {0,0,0};

    for (unsigned int i = 0; i < c->width * c->height; i++) {
        TupleEqual(c->__canvas[i], &color);
    }

    free_canvas(c);
}

void TestPixelWrite() {
    Canvas *c = canvas(10, 20);
    Tuple *red = vec(1, 0, 0);
    Equal(c->width, 10);
    Equal(c->height, 20);
    Tuple color = {0,0,0};

    write_pixel(c, 2, 3, red);
    TupleEqual(red, pixel_at(c, 2, 3));

    free(red);
    free_canvas(c);
}

void TestCanvasFeature() {
  Feature f = {"Canvas"};
  AddTest(&f, TestCanvasCreation, "Creating a Canvas");
  AddTest(&f, TestPixelWrite, "Writing pixels to a canvas");
  AddFeature(f);
}
