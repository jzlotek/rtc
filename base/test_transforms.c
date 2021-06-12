#include "transforms.h"
#include "../utils/consts.h"
#include "../utils/test.h"
#include <math.h>

#pragma once

void PointMatrixMult() {
    Matrix *t = translation(5, -3, 2);
    Matrix *t_inv = inverse(t);
    tuple *p = point(-3, 4, 5);
    tuple *exp = point(2, 1, 7);
    apply(p, t);
    TupleEqual(p, exp);
    free(exp);
    exp = point(-3, 4, 5);
    apply(p, t_inv);
    TupleEqual(p, exp);

    free(t); free(p); free(exp); free(t_inv);
}

void VecMatrixMult() {
    Matrix *t = translation(5, -3, 2);
    Matrix *t_inv = inverse(t);
    tuple *p = vec(-3, 4, 5);
    tuple *exp = vec(-3, 4, 5);
    apply(p, t);
    TupleEqual(p, exp);

    free(t); free(p); free(exp); free(t_inv);
}

void PointMatrixScale() {
    Matrix *t = scaling(2, 3, 4);
    tuple *p = point(-1, 3, 5);
    tuple *exp = point(-2, 9, 20);
    apply(p, t);
    TupleEqual(p, exp);

    free(t); free(p); free(exp);
}

void PointRotateX() {
    Matrix *half_quarter = rotation_x(M_PI_4);
    Matrix *full_quarter = rotation_x(M_PI_2);
    tuple *p = point(0, 1, 0);
    tuple *exp = point(0, sqrt(2) / 2, sqrt(2) / 2);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 1, 0);
    exp = point(0,0,1);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free(half_quarter); free(full_quarter);
}

void PointRotateY() {
    Matrix *half_quarter = rotation_y(M_PI_4);
    Matrix *full_quarter = rotation_y(M_PI_2);
    tuple *p = point(0, 0, 1);
    tuple *exp = point(sqrt(2) / 2, 0, sqrt(2) / 2);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 0, 1);
    exp = point(1, 0, 0);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free(half_quarter); free(full_quarter);
}

void PointRotateZ() {
    Matrix *half_quarter = rotation_z(M_PI_4);
    Matrix *full_quarter = rotation_z(M_PI_2);
    tuple *p = point(0, 1, 0);
    tuple *exp = point(-sqrt(2) / 2, sqrt(2) / 2, 0);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 1, 0);
    exp = point(-1, 0, 0);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free(half_quarter); free(full_quarter);
}

void __ShearPointHelper(tuple p, tuple exp, float a, float b, float c, float d, float e, float f) {
    Matrix *t = shear(a,b,c,d,e,f);
    TupleEqual(apply(&p, t), &exp);
    free(t);
}

void ShearPoint() {
    #define NUM_TESTS 6

    tuple points[NUM_TESTS] = {
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
    };
    tuple exp[NUM_TESTS] = {
        {5, 3, 4},
        {6, 3, 4},
        {2, 5, 4},
        {2, 7, 4},
        {2, 3, 6},
        {2, 3, 7},
    };
    for (unsigned int i = 0; i < NUM_TESTS; i++) {
        __ShearPointHelper(points[i], exp[i], i==0, i==1, i==2, i==3, i==4, i==5);
    }
}

void TestChainMatMul() {
    tuple *p = point(1, 0, 1);
    tuple *exp = point(15, 0, 7);
    Matrix *t = chain_matmul(3, rotation_x(M_PI_2), scaling(5, 5, 5), translation(10, 5, 7));
    apply(p, t);
    TupleEqual(p, exp);
    free(p); free(exp); free(t);
}

void TestTransformsFeature() {
  Feature f = {"Transforms"};
  AddTest(&f, PointMatrixMult, "Multiply point by matrix");
  AddTest(&f, VecMatrixMult, "Multiply vector by matrix");
  AddTest(&f, PointMatrixScale, "Scale vector by matrix");
  AddTest(&f, PointRotateX, "X Rotation");
  AddTest(&f, PointRotateY, "Y Rotation");
  AddTest(&f, PointRotateZ, "Z Rotation");
  AddTest(&f, ShearPoint, "Testing Shear");
  AddTest(&f, TestChainMatMul, "Testing Chain Multiplication");
  AddFeature(f);
}
