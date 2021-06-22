#include "../base/transforms.h"
#include "../utils/consts.h"
#include "test.h"
#include <math.h>

#pragma once

void PointMatrixMult() {
    Matrix *t = translation(5, -3, 2);
    Matrix *t_inv = inverse(t);
    Tuple *p = point(-3, 4, 5);
    Tuple *exp = point(2, 1, 7);
    apply(p, t);
    TupleEqual(p, exp);
    free(exp);
    exp = point(-3, 4, 5);
    apply(p, t_inv);
    TupleEqual(p, exp);

    free_matrix(t); free(p); free(exp); free_matrix(t_inv);
}

void VecMatrixMult() {
    Matrix *t = translation(5, -3, 2);
    Matrix *t_inv = inverse(t);
    Tuple *p = vec(-3, 4, 5);
    Tuple *exp = vec(-3, 4, 5);
    apply(p, t);
    TupleEqual(p, exp);

    free_matrix(t); free(p); free(exp); free_matrix(t_inv);
}

void PointMatrixScale() {
    Matrix *t = scaling(2, 3, 4);
    Tuple *p = point(-1, 3, 5);
    Tuple *exp = point(-2, 9, 20);
    apply(p, t);
    TupleEqual(p, exp);

    free_matrix(t); free(p); free(exp);
}

void PointRotateX() {
    Matrix *half_quarter = rotation_x(M_PI_4);
    Matrix *full_quarter = rotation_x(M_PI_2);
    Tuple *p = point(0, 1, 0);
    Tuple *exp = point(0, sqrt(2) / 2, sqrt(2) / 2);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 1, 0);
    exp = point(0,0,1);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free_matrix(half_quarter); free_matrix(full_quarter);
}

void PointRotateY() {
    Matrix *half_quarter = rotation_y(M_PI_4);
    Matrix *full_quarter = rotation_y(M_PI_2);
    Tuple *p = point(0, 0, 1);
    Tuple *exp = point(sqrt(2) / 2, 0, sqrt(2) / 2);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 0, 1);
    exp = point(1, 0, 0);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free_matrix(half_quarter); free_matrix(full_quarter);
}

void PointRotateZ() {
    Matrix *half_quarter = rotation_z(M_PI_4);
    Matrix *full_quarter = rotation_z(M_PI_2);
    Tuple *p = point(0, 1, 0);
    Tuple *exp = point(-sqrt(2) / 2, sqrt(2) / 2, 0);
    TupleEqual(apply(p, half_quarter), exp);
    free(exp); free(p);
    p = point(0, 1, 0);
    exp = point(-1, 0, 0);
    TupleEqual(apply(p, full_quarter), exp);

    free(p); free(exp); free_matrix(half_quarter); free_matrix(full_quarter);
}

void __ShearPointHelper(Tuple p, Tuple exp, float a, float b, float c, float d, float e, float f) {
    Matrix *t = shear(a,b,c,d,e,f);
    TupleEqual(apply(&p, t), &exp);
    free_matrix(t);
}

void ShearPoint() {
    Tuple points[6] = {
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
        {2, 3, 4},
    };
    Tuple exp[6] = {
        {5, 3, 4},
        {6, 3, 4},
        {2, 5, 4},
        {2, 7, 4},
        {2, 3, 6},
        {2, 3, 7},
    };
    for (unsigned int i = 0; i < 6; i++) {
        __ShearPointHelper(points[i], exp[i], i==0, i==1, i==2, i==3, i==4, i==5);
    }
}

void TestChainMatMul() {
    Tuple *p = point(1, 0, 1);
    Tuple *exp = point(15, 0, 7);
    Matrix *t = chain_matmul(3, rotation_x(M_PI_2), scaling(5, 5, 5), translation(10, 5, 7));
    apply(p, t);
    TupleEqual(p, exp);
    free(p); free(exp); free_matrix(t);
}

void TestViewTransformIdent() {
    Matrix *ident = I();
    Matrix *res = view_transform(point(0,0,0), point(0,0,-1),vec(0,1,0));
    True(mateq(ident, res));
    free_matrix(ident); free_matrix(res);
}

void TestViewTransformZ() {
    Matrix *exp = scaling(-1, 1, -1);
    Matrix *res = view_transform(point(0,0,0), point(0,0,1),vec(0,1,0));
    True(mateq(exp, res));
    free_matrix(exp); free_matrix(res);
}

void TestViewTransformTranslate() {
    Matrix *exp = translation(0,0,-8);
    Matrix *res = view_transform(point(0,0,8), point(0,0,0),vec(0,1,0));
    True(mateq(exp, res));
    free_matrix(exp); free_matrix(res);
}

void TestViewTransformArbitrary() {
    float data[4][4] = {
        { -0.50709 , 0.50709 , 0.67612 , -2.36643 },
        { 0.76772 , 0.60609 , 0.12122 , -2.82843 },
        { -0.35857 , 0.59761 , -0.71714 , 0.00000 },
        { 0.00000 , 0.00000 , 0.00000 , 1.00000 }
    };
    Matrix *exp = matrix(data, 4, 4);
    Matrix *res = view_transform(point(1,3,2), point(4,-2,8),vec(1,1,0));
    True(mateq(exp, res));
    free_matrix(exp); free_matrix(res);
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
  AddTest(&f, TestViewTransformIdent, "The transformation matrix for the default orientation");
  AddTest(&f, TestViewTransformZ, "A view transformation matrix looking in positive z direction");
  AddTest(&f, TestViewTransformTranslate, "The view transformation moves the world");
  AddTest(&f, TestViewTransformArbitrary, "An arbitrary view transformation");
  AddFeature(f);
}
