#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../utils/consts.h"

#pragma once

typedef struct {
    float x,y,z,w;
} tuple;

bool isPoint(tuple *t);
bool isVector(tuple *t);
void print_tuple(tuple *t);
tuple *copy_from(tuple *dest, const tuple *src);
bool equal(const tuple *t1, const tuple *t2);
tuple *point(float x, float y, float z);
tuple *vec(float x, float y, float z);
tuple *add(tuple *t1, const tuple *t2);
tuple *sub(tuple *t1, const tuple *t2);
tuple *mult(tuple *t1, float c);
tuple *divide(tuple *t1, float c);
tuple *prod(tuple *t1, const tuple *t2);
tuple *cross(const tuple *t1, const tuple *t2);
float dot(tuple *t1, tuple *t2);
tuple *norm(tuple *t);
float magnitude(const tuple *t);

bool isPoint(tuple *t) {
    return fabs(t->w - 1.0f) < EPSILON;
}

bool isVector(tuple *t) {
    return fabs(t->w) < EPSILON;
}

void print_tuple(tuple *t) {
    if (isPoint(t)) {
        printf("(%.6f, %6f, %.6f, %.6f)", t->x, t->y, t->z, t->w);
    } else {
        printf("<%.6f, %6f, %.6f>", t->x, t->y, t->z);
    }
}

tuple *copy_from(tuple *dest, const tuple *src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
    dest->w = src->w;
    return dest;
}

bool equal(const tuple *t1, const tuple *t2) {
    // TODO: SIMD
    return
        fabs(t1->x - t2->x) < EPSILON &&
        fabs(t1->y - t2->y) < EPSILON &&
        fabs(t1->z - t2->z) < EPSILON &&
        fabs(t1->w - t2->w) < EPSILON;
}

tuple *point(float x, float y, float z) {
    tuple *r = (tuple*)malloc(sizeof(tuple));
    r->x = x;
    r->y = y;
    r->z = z;
    r->w = 1.0;
    return r;
}

tuple *vec(float x, float y, float z) {
    tuple *r = (tuple*)malloc(sizeof(tuple));
    r->x = x;
    r->y = y;
    r->z = z;
    r->w = .0;
    return r;
}

tuple *add(tuple *t1, const tuple *t2) {
    t1->x += t2->x;
    t1->y += t2->y;
    t1->z += t2->z;
    t1->w += t2->w;
    return t1;
}

tuple *sub(tuple *t1, const tuple *t2) {
    t1->x -= t2->x;
    t1->y -= t2->y;
    t1->z -= t2->z;
    t1->w -= t2->w;
    return t1;
}

tuple *neg(tuple *t1) {
    t1->x = -t1->x;
    t1->y = -t1->y;
    t1->z = -t1->z;
    t1->w = -t1->w;
    return t1;
}

tuple *mult(tuple *t1, float c) {
    t1->x *= c;
    t1->y *= c;
    t1->z *= c;
    t1->w *= c;
    return t1;
}

tuple *prod(tuple *t1, const tuple *t2) {
    t1->x *= t2->x;
    t1->y *= t2->y;
    t1->z *= t2->z;
    t1->w *= t2->w;
    return t1;
}

tuple *divide(tuple *t1, float c) {
    t1->x /= c;
    t1->y /= c;
    t1->z /= c;
    t1->w /= c;
    return t1;
}

float magnitude(const tuple *t){
    float a = t->x * t->x;
    float b = t->y * t->y;
    float c = t->z * t->z;
    float d = t->w * t->w;

    return sqrt(a + b + c + d);
}

tuple *norm(tuple *t) {
    float d = magnitude(t);
    divide(t, d);
    return t;
}

float dot(tuple *t1, tuple *t2) {
    float a = t1->x * t2->x;
    float b = t1->y * t2->y;
    float c = t1->z * t2->z;
    float d = t1->w * t2->w;
    return a + b + c + d;
}

tuple *cross(const tuple *a, const tuple *b) {
    if (a->w != 0 || b->w != 0) {
        perror("Cannot call cross on points (w must be equal to 0)");
        exit(1);
    }
    tuple *res = vec(0,0,0);
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
    return res;
}
