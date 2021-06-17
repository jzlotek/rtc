#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

typedef struct {
    float data[4][4];
    unsigned int rows;
    unsigned int cols;
} Matrix;

Matrix *matrix(float data[4][4], unsigned int rows, unsigned int cols);
Matrix *copy(const Matrix *src);
Tuple *apply(Tuple *point, const Matrix *transform);
bool mateq(const Matrix *m1, const Matrix *m2);
Matrix *matmul(const Matrix *m1, const Matrix *m2);
Matrix *I();
Matrix *transpose(Matrix *m);
Matrix *submatrix(const Matrix *a, unsigned int r, unsigned int c);
float det(const Matrix *a);
float minor(const Matrix *a, unsigned int i, unsigned int j);
float cofactor(const Matrix *a, unsigned int i, unsigned int j);
Matrix *inverse(const Matrix *m);

Matrix *matrix(float data[4][4], unsigned int rows, unsigned int cols) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            m->data[i][j] = data[i][j];
        }
    }
    m->cols = cols;
    m->rows = rows;
    return m;
}

Matrix *copy(const Matrix *src) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    for (unsigned int i = 0; i < src->rows; i++) {
        for (unsigned int j = 0; j < src->cols; j++) {
            m->data[i][j] = src->data[i][j];
        }
    }
    m->cols = src->cols;
    m->rows = src->rows;
    return m;
}

float __mult_row(Tuple *t, const Matrix *m, unsigned int r) {
    return m->data[r][0] * t->x +
        m->data[r][1] * t->y +
        m->data[r][2] * t->z +
        m->data[r][3] * t->w;
}

Tuple *apply(Tuple *point, const Matrix *transform) {
    Tuple *tmp = vec(0, 0, 0);
    tmp->x = __mult_row(point, transform, 0);
    tmp->y = __mult_row(point, transform, 1);
    tmp->z = __mult_row(point, transform, 2);
    tmp->w = __mult_row(point, transform, 3);
    copy_from(point, tmp);
    free(tmp);
    return point;
}

bool mateq(const Matrix *m1, const Matrix *m2) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) return false;

    for (unsigned int i = 0; i < m1->rows; i++) {
        for (unsigned int j = 0; j < m1->cols; j++) {
            if (fabs(m1->data[i][j] - m2->data[i][j]) >= EPSILON) {
                return false;
            }
        }
    }

    return true;
}

Matrix *matmul(const Matrix *m1, const Matrix *m2) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) perror("cannot multiply matricies of (n*m) and (j*k) where m!=j");
    float data[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    Matrix *result = matrix(data, 4, 4);

    for (unsigned int i = 0; i < m1->rows; i++) {
        for (unsigned int j = 0; j < m1->cols; j++) {
            for (unsigned int k = 0; k < m2->cols; k++) {
                result->data[i][k] += m1->data[i][j] * m2->data[j][k];
            }
        }
    }

    return result;
}

Matrix *I() {
    float data[4][4] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
    return matrix(data, 4, 4);
}

void print_matrix(Matrix *m) {
    for (unsigned int i = 0; i < m->rows; i++) {
        printf("[ ");
        for (unsigned int j = 0; j < m->cols; j++) {
            printf("%0.6f ", m->data[i][j]);
        }
        printf("]\n");
    }
}

Matrix *transpose(Matrix *m) {
    for (unsigned int i = 0; i < m->rows; i++) {
        for (unsigned int j = 0; j < i; j++) {
            float tmp = m->data[i][j];
            m->data[i][j] = m->data[j][i];
            m->data[j][i] = tmp;
        }
    }
    return m;
}

Matrix *submatrix(const Matrix *a, unsigned int r, unsigned int c) {
    float data[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    Matrix *newMat = matrix(data, a->rows - 1, a->cols - 1);
    int currRow = 0, currCol;
    for (unsigned int i = 0; i < a->rows; i++) {
        currCol = 0;
        if (i == r) continue;
        for (unsigned int j = 0; j < a->cols; j++) {
            if (j == c) continue;
            newMat->data[currRow][currCol] = a->data[i][j];
            currCol++;
        }
        currRow++;
    }

    return newMat;
}

float det(const Matrix *a) {
    if (a->rows == 2) {
        return a->data[0][0] * a->data[1][1] - a->data[1][0] * a->data[0][1];
    }

    float determinate = 0;

    for (unsigned int i = 0; i < a->rows; i++) {
        determinate += a->data[i][0] * cofactor(a, i, 0);
    }

    return determinate;
}


float minor(const Matrix *a, unsigned int i, unsigned int j) {
    Matrix *sub = submatrix(a, i, j);
    float d = det(sub);
    free(sub);
    return d;
}

float cofactor(const Matrix *a, unsigned int i, unsigned int j) {
    return minor(a, i, j) * ((i + j) % 2 == 1 ? -1 : 1);
}

Matrix *inverse(const Matrix *m) {
    float d = det(m);
    if (d == 0) {
        perror("matrix is not inveritble");
        exit(1);
    }
    float data[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    Matrix *ret = matrix(data, m->rows, m->cols);

    float c;

    for (unsigned int row = 0; row < m->rows; row++) {
        for (unsigned int col = 0; col < m->cols; col++) {
            c = cofactor(m, row, col);
            ret->data[col][row] = c / d;
        }
    }

    return ret;
}