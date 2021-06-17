#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../utils/consts.h"

#pragma once

typedef struct {
    float x,y,z,w;
} Tuple;

bool isPoint(Tuple *t);
bool isVector(Tuple *t);
void print_tuple(Tuple *t);
Tuple *copy_tuple(const Tuple *src);
Tuple *copy_from(Tuple *dest, const Tuple *src);
bool equal(const Tuple *t1, const Tuple *t2);
Tuple *point(float x, float y, float z);
Tuple *vec(float x, float y, float z);
Tuple *add(Tuple *t1, const Tuple *t2);
Tuple *sub(Tuple *t1, const Tuple *t2);
Tuple *mult(Tuple *t1, float c);
Tuple *divide(Tuple *t1, float c);
Tuple *prod(Tuple *t1, const Tuple *t2);
Tuple *cross(const Tuple *t1, const Tuple *t2);
float dot(const Tuple *t1, const Tuple *t2);
Tuple *norm(Tuple *t);
float magnitude(const Tuple *t);
Tuple *reflect(Tuple *a, const Tuple *b);

bool isPoint(Tuple *t) {
    return fabs(t->w - 1.0f) < EPSILON;
}

bool isVector(Tuple *t) {
    return fabs(t->w) < EPSILON;
}

void print_tuple(Tuple *t) {
    if (isPoint(t)) {
        printf("(%.6f, %6f, %.6f, %.6f)", t->x, t->y, t->z, t->w);
    } else {
        printf("<%.6f, %6f, %.6f>", t->x, t->y, t->z);
    }
}

Tuple *copy_tuple(const Tuple *src) {
    Tuple *t = point(0,0,0);
    return copy_from(t, src);
}

Tuple *copy_from(Tuple *dest, const Tuple *src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
    dest->w = src->w;
    return dest;
}

bool equal(const Tuple *t1, const Tuple *t2) {
    // TODO: SIMD
    return
        fabs(t1->x - t2->x) < EPSILON &&
        fabs(t1->y - t2->y) < EPSILON &&
        fabs(t1->z - t2->z) < EPSILON &&
        fabs(t1->w - t2->w) < EPSILON;
}

Tuple *point(float x, float y, float z) {
    Tuple *r = (Tuple*)malloc(sizeof(Tuple));
    r->x = x;
    r->y = y;
    r->z = z;
    r->w = 1.0;
    return r;
}

Tuple *vec(float x, float y, float z) {
    Tuple *r = (Tuple*)malloc(sizeof(Tuple));
    r->x = x;
    r->y = y;
    r->z = z;
    r->w = .0;
    return r;
}

Tuple *add(Tuple *t1, const Tuple *t2) {
    t1->x += t2->x;
    t1->y += t2->y;
    t1->z += t2->z;
    t1->w += t2->w;
    return t1;
}

Tuple *sub(Tuple *t1, const Tuple *t2) {
    t1->x -= t2->x;
    t1->y -= t2->y;
    t1->z -= t2->z;
    t1->w -= t2->w;
    return t1;
}

Tuple *neg(Tuple *t1) {
    t1->x = -t1->x;
    t1->y = -t1->y;
    t1->z = -t1->z;
    t1->w = -t1->w;
    return t1;
}

Tuple *mult(Tuple *t1, float c) {
    t1->x *= c;
    t1->y *= c;
    t1->z *= c;
    t1->w *= c;
    return t1;
}

Tuple *prod(Tuple *t1, const Tuple *t2) {
    t1->x *= t2->x;
    t1->y *= t2->y;
    t1->z *= t2->z;
    t1->w *= t2->w;
    return t1;
}

Tuple *divide(Tuple *t1, float c) {
    t1->x /= c;
    t1->y /= c;
    t1->z /= c;
    t1->w /= c;
    return t1;
}

float magnitude(const Tuple *t){
    float a = t->x * t->x;
    float b = t->y * t->y;
    float c = t->z * t->z;
    float d = t->w * t->w;

    return sqrt(a + b + c + d);
}

Tuple *norm(Tuple *t) {
    return divide(t, magnitude(t));
}

float dot(const Tuple *t1, const Tuple *t2) {
    float a = t1->x * t2->x;
    float b = t1->y * t2->y;
    float c = t1->z * t2->z;
    float d = t1->w * t2->w;
    return a + b + c + d;
}

Tuple *cross(const Tuple *a, const Tuple *b) {
    if (a->w != 0 || b->w != 0) {
        perror("Cannot call cross on points (w must be equal to 0)");
        exit(1);
    }
    Tuple *res = vec(0,0,0);
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
    return res;
}

Tuple *reflect(Tuple *a, const Tuple *b) {
    Tuple *b_cpy = copy_tuple(b);
    Tuple *res =  sub(a, mult(b_cpy, 2 * dot(a, b)));
    free(b_cpy);
    return res;
}