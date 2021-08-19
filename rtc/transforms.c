#include <stdarg.h>

#include "../rtc.h"

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
        free_matrix(tmp); free_matrix(next);
    }

    va_end(va);
    return acc;
}

Matrix *view_transform(Tuple *from, Tuple *to, Tuple *up) {
    // will take in 3 arguments that will be thrown out after
    Tuple *forward = norm(sub(to, from));
    Tuple *left = cross(forward, norm(up));
    Tuple *true_up = cross(left, forward);
    Matrix *orientation = I();
    orientation->data[0][0] = left->vals[0];
    orientation->data[0][1] = left->vals[1];
    orientation->data[0][2] = left->vals[2];
    orientation->data[1][0] = true_up->vals[0];
    orientation->data[1][1] = true_up->vals[1];
    orientation->data[1][2] = true_up->vals[2];
    orientation->data[2][0] = -forward->vals[0];
    orientation->data[2][1] = -forward->vals[1];
    orientation->data[2][2] = -forward->vals[2];

    Matrix *res = chain_matmul(2, translation(-from->vals[0], -from->vals[1], -from->vals[2]), orientation);
    free(from); free(to); free(up); free(left); free(true_up);
    return res;
}
