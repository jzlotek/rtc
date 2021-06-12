#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "matrix.h"
#include "tuple.h"

#pragma once

tuple *apply(tuple *point, const Matrix *transform);
Matrix *translation(float x, float y, float z);
Matrix *scaling(float x, float y, float z);
Matrix *rotation_x(float r);
Matrix *rotation_y(float r);
Matrix *rotation_z(float r);
Matrix *shear(float xy, float xz, float yx, float yz, float zx, float zy);
Matrix *chain_matmul(int count, ...);

float __mult_row(tuple *t, const Matrix *m, unsigned int r) {
    return m->data[r][0] * t->x +
        m->data[r][1] * t->y +
        m->data[r][2] * t->z +
        m->data[r][3] * t->w;
}

tuple *apply(tuple *point, const Matrix *transform) {
    tuple *tmp = vec(0, 0, 0);
    tmp->x = __mult_row(point, transform, 0);
    tmp->y = __mult_row(point, transform, 1);
    tmp->z = __mult_row(point, transform, 2);
    tmp->w = __mult_row(point, transform, 3);
    copy_from(point, tmp);
    free(tmp);
    return point;
}

Matrix *translation(float x, float y, float z) {
    Matrix *ret = I();
    ret->data[0][3] = x;
    ret->data[1][3] = y;
    ret->data[2][3] = z;
    return ret;
}

Matrix *scaling(float x, float y, float z) {
    Matrix *ret = I();
    ret->data[0][0] = x;
    ret->data[1][1] = y;
    ret->data[2][2] = z;
    return ret;
}

Matrix *rotation_x(float r) {
    Matrix *ret = I();
    ret->data[1][1] = cos(r);
    ret->data[1][2] = -sin(r);
    ret->data[2][1] = sin(r);
    ret->data[2][2] = cos(r);
    return ret;
}

Matrix *rotation_y(float r) {
    Matrix *ret = I();
    ret->data[0][0] = cos(r);
    ret->data[2][0] = -sin(r);
    ret->data[0][2] = sin(r);
    ret->data[2][2] = cos(r);
    return ret;
}

Matrix *rotation_z(float r) {
    Matrix *ret = I();
    ret->data[0][0] = cos(r);
    ret->data[0][1] = -sin(r);
    ret->data[1][0] = sin(r);
    ret->data[1][1] = cos(r);
    return ret;
}

Matrix *shear(float xy, float xz, float yx, float yz, float zx, float zy) {
    Matrix *ret = I();
    ret->data[0][1] = xy;
    ret->data[0][2] = xz;
    ret->data[1][0] = yx;
    ret->data[1][2] = yz;
    ret->data[2][0] = zx;
    ret->data[2][1] = zy;
    return ret;
}

Matrix *chain_matmul(int count, ...) {
    int i;
    Matrix *acc = I();
    Matrix *tmp;
    va_list va;
    va_start(va, count);

    for (i = 0; i < count; i++) {
        tmp = acc;
        Matrix *next = va_arg(va, Matrix*);
        acc = matmul(next, tmp);
        free(tmp); free(next);
    }

    va_end(va);
    return acc;
}