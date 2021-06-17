#include "../base/matrix.h"
#include "../utils/consts.h"
#include "test.h"

#pragma once

void Matrix2Construction() {
    float data[4][4] = {
        {-3, 5},
        {1, -2}
    };
    Matrix *mat = matrix(data, 2, 2);
    Equal(mat->data[0][0], -3);
    Equal(mat->data[0][1], 5);
    Equal(mat->data[1][0], 1);
    Equal(mat->data[1][1], -2);
    free(mat);
}

void Matrix3Construction() {
    float data[4][4] = {
        {-3, 5, 0},
        {1, -2, -7},
        {0, 1, -1}
    };
    Matrix *mat = matrix(data, 3, 3);
    Equal(mat->data[0][0], -3);
    Equal(mat->data[1][1], -2);
    Equal(mat->data[2][2], -1);
    free(mat);
}

void Matrix4Construction() {
    float data[4][4] = {
        {1 , 2 , 3 , 4},
        {5.5 , 6.5 , 7.5 , 8.5},
        {9 , 10 , 11 , 12},
        {13.5 , 14.5 , 15.5 , 16.5}
    };
    Matrix *mat = matrix(data, 4, 4);
    Equal(mat->data[0][0], 1);
    Equal(mat->data[0][3], 4);
    Equal(mat->data[1][0], 5.5);
    Equal(mat->data[1][2], 7.5);
    Equal(mat->data[2][2], 11);
    Equal(mat->data[3][0], 13.5);
    Equal(mat->data[3][2], 15.5);
    free(mat);
}

void Matrix4Eq() {
    float data[4][4] = {
        {1 , 2 , 3 , 4},
        {5.5 , 6.5 , 7.5 , 8.5},
        {9 , 10 , 11 , 12},
        {13.5 , 14.5 , 15.5 , 16.5}
    };
    float data2[4][4] = {
        {1 , 2 , 3 , 4},
        {5.5 , 6.5 , 7.5 , 8.5},
        {9 , 10 , 11 , 12},
        {13.5 , 14.5 , 15.5 , 17.5}
    };
    Matrix *mat = matrix(data, 4, 4);
    Matrix *mat2 = matrix(data2, 4, 4);
    True(mateq(mat, mat));
    False(mateq(mat, mat2));
    free(mat); free(mat2);
}

void MatrixMultiply() {
    float data[4][4] = {
        { 1 , 2 , 3 , 4 },
        { 5 , 6 , 7 , 8 },
        { 9 , 8 , 7 , 6 },
        { 5 , 4 , 3 , 2 }
    };
    float data2[4][4] = {
        { -2 , 1 , 2 , 3 },
        { 3 , 2 , 1 , -1 },
        { 4 , 3 , 6 , 5 },
        { 1 , 2 , 7 , 8 }
    };
    float data3[4][4] = {
        { 20, 22 , 50 , 48 },
        { 44, 54 , 114 , 108 },
        { 40, 58 , 110 , 102 },
        { 16, 26 , 46 , 42 }
    };
    Matrix *mat = matrix(data, 4, 4);
    Matrix *mat2 = matrix(data2, 4, 4);
    Matrix *exp = matrix(data3, 4, 4);
    Matrix *res = matmul(mat, mat2);
    True(mateq(res, exp));

    free(mat); free(mat2); free(exp); free(res);
}

void MatTupleMult() {
    float data[4][4] = {
        { 1 , 2 , 3 , 4 },
        { 2 , 4 , 4 , 2 },
        { 8 , 6 , 4 , 1 },
        { 0 , 0 , 0 , 1 }
    };
    Matrix *mat = matrix(data, 4, 4);
    Tuple *p = point(1,2,3);
    Tuple *exp = point(18, 24, 33);

    TupleEqual(apply(p, mat), exp);

    free(mat); free(exp); free(p);
}

void MatrixMultiplyI() {
    float data[4][4] = {
        { 1 , 2 , 3 , 4 },
        { 5 , 6 , 7 , 8 },
        { 9 , 8 , 7 , 6 },
        { 5 , 4 , 3 , 2 }
    };
    Matrix *mat = matrix(data, 4, 4);
    Matrix *ident = I();
    Matrix *res = matmul(mat, ident);
    True(mateq(mat, res));

    free(mat); free(ident); free(res);
}

void MatrixTranspose() {
    float data[4][4] = {
        { 1 , 2 , 3 , 4 },
        { 5 , 6 , 7 , 8 },
        { 9 , 8 , 7 , 6 },
        { 5 , 4 , 3 , 2 }
    };
    Matrix *mat = matrix(data, 4, 4);

    float data2[4][4] = {
        { 1 , 5 , 9 , 5 },
        { 2 , 6 , 8 , 4 },
        { 3 , 7 , 7 , 3 },
        { 4 , 8 , 6 , 2 }
    };
    Matrix *mat_T = matrix(data2, 4, 4);

    True(mateq(transpose(mat), mat_T));

    Matrix *ident = I();
    Matrix *icopy = copy(ident);
    True(mateq(transpose(ident), icopy));

    free(mat); free(ident); free(icopy); free(mat_T);
}

void Submatrix() {
    float data[4][4] = {
        {1, 5, 0},
        {-3, 2, 7},
        {0, 6, -3}
    };
    Matrix *mat = matrix(data, 3, 3);
    float data2[4][4] = {
        {-3, 2},
        {0, 6}
    };
    Matrix *res = matrix(data2, 2, 2);
    Matrix *sub = submatrix(mat, 0, 2);
    True(mateq(res, sub));
    free(mat); free(sub); free(res);
}

void Matrix2Determinate() {
    float data[4][4] = {
        {1, 5},
        {-3, 2}
    };
    Matrix *mat = matrix(data, 2, 2);
    Equal(det(mat), 17);
    free(mat);
}

void MatrixMinor() {
    float data[4][4] = {
        {3, 5, 0},
        {2, -1, -7},
        {6, -1, 5}
    };
    Matrix *mat = matrix(data, 3, 3);
    Matrix *b = submatrix(mat, 1, 0);
    Equal(det(b), 25);
    Equal(minor(mat, 1, 0), 25);
    free(mat); free(b);
}

void MatrixCofactor() {
    float data[4][4] = {
        {3, 5, 0},
        {2, -1, -7},
        {6, -1, 5}
    };
    Matrix *mat = matrix(data, 3, 3);
    Equal(minor(mat, 0, 0), -12);
    Equal(cofactor(mat, 0, 0), -12);
    Equal(minor(mat, 1, 0), 25);
    Equal(cofactor(mat, 1, 0), -25);
    free(mat);
}

void MatrixDeterminate() {
    float data[4][4] = {
        {-2, -8, 3, 5},
        {-3, 1, 7, 3},
        {1, 2, -9, 6},
        {-6, 7, 7, -9}
    };
    Matrix *mat = matrix(data, 4, 4);

    Equal(cofactor(mat, 0, 0), 690);
    Equal(cofactor(mat, 0, 1), 447);
    Equal(cofactor(mat, 0, 2), 210);
    Equal(cofactor(mat, 0, 3), 51);
    Equal(det(mat), -4071);
    free(mat);
}

void MatrixInvertibility() {
    float data[4][4] = {
        {6, 4, 4, 4},
        {5, 5, 7, 6},
        {0, -5, 1, -5},
        {9, 1, 7, -6}
    };
    Matrix *mat = matrix(data, 4, 4);
    float data2[4][4] = {
        {-4, 2, -2, -3},
        {9, 6, 2, 6},
        {0, -5, 1, -5},
        {0, 0, 0, 0}
    };
    Matrix *mat2 = matrix(data2, 4, 4);

    True(det(mat) != 0);
    True(det(mat2) == 0);
    
    free(mat); free(mat2);
}

void MatrixInverting() {
    float data[4][4] = {
        { 8 , -5 , 9 , 2 },
        { 7 , 5 , 6 , 1 },
        { -6 , 0 , 9 , 6 },
        { -3 , 0 , -9 , -4 }
    };
    Matrix *mat = matrix(data, 4, 4);
    float data2[4][4] = {
        { -0.15385 , -0.15385 , -0.28205 , -0.53846 },
        { -0.07692 , 0.12308 , 0.02564 , 0.03077 },
        { 0.35897 , 0.35897 , 0.43590 , 0.92308 },
        { -0.69231 , -0.69231 , -0.76923 , -1.92308 }
    };
    Matrix *exp = matrix(data2, 4, 4);

    Matrix *res = inverse(mat);

    True(mateq(res, exp));

    free(mat); free(res); free(exp);
}

void MatrixInvertingAssociative() {
    float data[4][4] = {
        { 8 , -5 , 9 , 2 },
        { 7 , 5 , 6 , 1 },
        { -6 , 0 , 9 , 6 },
        { -3 , 0 , -9 , -4 }
    };
    Matrix *mat = matrix(data, 4, 4);
    float data2[4][4] = {
        { -0.15385 , -0.15385 , -0.28205 , -0.53846 },
        { -0.07692 , 0.12308 , 0.02564 , 0.03077 },
        { 0.35897 , 0.35897 , 0.43590 , 0.92308 },
        { -0.69231 , -0.69231 , -0.76923 , -1.92308 }
    };
    Matrix *exp = matrix(data2, 4, 4);

    Matrix *res = inverse(mat);
    Matrix *ident = I();

    True(mateq(res, exp));
    Matrix *mmres = matmul(mat, res);
    True(mateq(mmres, ident));

    free(mat); free(res); free(exp); free(ident); free(mmres);
}

void TestMatrixFeature() {
  Feature f = {"Matrix"};
  AddTest(&f, Matrix2Construction, "Constructing and inspecting a 2x2 matrix");
  AddTest(&f, Matrix3Construction, "Constructing and inspecting a 3x3 matrix");
  AddTest(&f, Matrix4Construction, "Constructing and inspecting a 4x4 matrix");
  AddTest(&f, Matrix4Eq, "Matrix equality");
  AddTest(&f, MatrixMultiply, "Matrix multiply");
  AddTest(&f, MatrixMultiplyI, "Matrix multiply identity");
  AddTest(&f, MatrixTranspose, "Matrix transpose");
  AddTest(&f, Submatrix, "A submatrix of a 3x3 matrix is a 2x2 matrix");
  AddTest(&f, MatTupleMult, "matrix multiplied by a tuple");
  AddTest(&f, Matrix2Determinate, "Calculating the determinant of a 2x2 matrix");
  AddTest(&f, MatrixMinor, "Calculating a minor of a 3x3 matrix");
  AddTest(&f, MatrixCofactor, "Calculating a cofactor of a 3x3 matrix");
  AddTest(&f, MatrixDeterminate, "Calculating a determinate of a 4x4 matrix");
  AddTest(&f, MatrixInvertibility, "Testing an invertible matrix for invertibility");
  AddTest(&f, MatrixInverting, "Calculating the inverse of matricies");
  AddTest(&f, MatrixInvertingAssociative, "Associativity of Matricies");
  AddFeature(f);
}
