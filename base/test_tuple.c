#include <math.h>
#include "tuple.h"
#include "../utils/consts.h"
#include "../utils/test.h"

void TestPoint() {
  tuple t = {4.3, -4.2, 3.1, 1.0};
  Equal(t.x, 4.3);
  Equal(t.y, -4.2);
  Equal(t.z, 3.1);
  Equal(t.w, 1.0);
  True(isPoint(&t));
  False(isVector(&t));
}

void TestVector() {
  tuple t = {4.3, -4.2, 3.1, 0.0};
  Equal(t.x, 4.3);
  Equal(t.y, -4.2);
  Equal(t.z, 3.1);
  Equal(t.w, 0.0);
  False(isPoint(&t));
  True(isVector(&t));
}

void TestPointFactory() {
  tuple *t = point(4, -4, 3);
  Equal(t->x, 4);
  Equal(t->y, -4);
  Equal(t->z, 3);
  Equal(t->w, 1);
  True(isPoint(t));
  False(isVector(t));
}

void TestVectorFactory() {
  tuple *t = vec(4, -4, 3);
  Equal(t->x, 4);
  Equal(t->y, -4);
  Equal(t->z, 3);
  Equal(t->w, 0);
  False(isPoint(t));
  True(isVector(t));
}

void TestTupleAdd() {
  tuple *t1 = point(3, -2, 5);
  tuple *t2 = vec(-2, 3, 1);
  add(t1, t2);
  Equal(t1->x, 1);
  Equal(t1->y, 1);
  Equal(t1->z, 6);
  Equal(t1->w, 1);
}

void TestPointSub() {
  tuple *t1 = point(3, 2, 1);
  tuple *t2 = point(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isVector(t1));
}

void TestTupleSub() {
  tuple *t1 = point(3, 2, 1);
  tuple *t2 = vec(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isPoint(t1));
}

void TestVectorSub() {
  tuple *t1 = vec(3, 2, 1);
  tuple *t2 = vec(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isVector(t1));
}

void TestZeroSub() {
  tuple *t1 = vec(0, 0, 0);
  tuple *t2 = vec(1, -2, 3);
  sub(t1, t2);
  Equal(t1->x, -1);
  Equal(t1->y, 2);
  Equal(t1->z, -3);
  True(isVector(t1));
}

void TestNegation() {
  tuple t1 = {1, -2, 3, -4};
  neg(&t1);
  tuple a = {-1, 2, -3, 4};
  TupleEqual(&t1, &a);
}

void TestMultiply() {
  tuple t1 = {1, -2, 3, -4};
  mult(&t1, 3.5);
  tuple a = {3.5, -7, 10.5, -14};
  TupleEqual(&t1, &a);
}

void TestMultiplyFrac() {
  tuple t1 = {1, -2, 3, -4};
  mult(&t1, 0.5);
  tuple a = {0.5, -1, 1.5, -2};
  TupleEqual(&t1, &a);
}

void TestDivide() {
  tuple t1 = {1, -2, 3, -4};
  divide(&t1, 2.0);
  tuple a = {0.5, -1, 1.5, -2};
  TupleEqual(&t1, &a);
}

void TestMagnitude() {
    tuple tuples[5] = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,2,3},
        {-1,-2,-3},
    };
    float res[5] = {1.0, 1.0, 1.0, sqrt(14), sqrt(14)};

    for (unsigned int i = 0; i < 5; i++) {
        Equal(magnitude(&tuples[i]), res[i]);
    }
}

void TestNormalize() {
    tuple tuples[2] = {
        {4,0,0},
        {1,2,3},
    };
    tuple res[2] = {
      {1,0,0},
      {0.26726, 0.53452, 0.80178},
    };

    for (unsigned int i = 0; i < 2; i++) {
        norm(&tuples[i]);
        Equal(magnitude(&tuples[i]), 1);
        TupleEqual(&tuples[i], &res[i]);
    }
}

void TestDot() {
  tuple *a = vec(1,2,3);
  tuple *b = vec(2,3,4);
  Equal(dot(a,b), 20);
  free(a);free(b);
}

void TestCross() {
  tuple *a = vec(1,2,3);
  tuple *b = vec(2,3,4);
  tuple *c1 = cross(a,b);
  tuple *c2 = cross(b,a);
  tuple d = {-1, 2, -1};
  tuple e = {1, -2, 1};
  TupleEqual(c1, &d);
  TupleEqual(c2, &e);
  free(a);free(b);free(c1);free(c2);
}

void __tick(tuple *p_pos, tuple *p_vel, tuple *env_g, tuple *env_wind) {
  add(p_pos, p_vel);
  add(p_vel, env_g);
  add(p_vel, env_wind);
}

void TestCannon() {
  tuple *p_pos, *p_vel, *env_g, *env_wind;
  p_pos = point(0,1.0,0);
  p_vel = norm(vec(1.0, 1.0, 0));
  env_g = vec(0, -0.1, 0);
  env_wind = vec(-0.01, 0, 0);

  int i = 0;
  while (p_pos->y > 0) {
    __tick(p_pos, p_vel, env_g, env_wind);
    i++;
  }

  free(p_pos); free(p_vel); free(env_g); free(env_wind);
  Equal(i, 17);
}

void TestColorAdd() {
  tuple *c1 = vec(0.9, 0.6, 0.75);
  tuple *c2 = vec(0.7, 0.1, 0.25);
  tuple color = {1.6, 0.7, 1.0};
  add(c1,c2);
  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestColorSub() {
  tuple *c1 = vec(0.9, 0.6, 0.75);
  tuple *c2 = vec(0.7, 0.1, 0.25);
  tuple color = {0.2, 0.5, 0.5};
  sub(c1,c2);
  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestColorMult() {
  tuple *c1 = vec(0.2, 0.3, 0.4);
  tuple color = {0.4, 0.6, 0.8};
  mult(c1, 2.0);
  TupleEqual(c1, &color);

  free(c1);
}

void TestColorProd() {
  tuple *c1 = vec(1.0, 0.2, 0.4);
  tuple *c2 = vec(0.9, 1.0, 0.1);
  tuple color = {0.9, 0.2, 0.04};
  prod(c1,c2);

  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestTupleFeature() {
  Feature f = {"Tuples, Vectors, and Points"};
  AddTest(&f, TestPoint, "A tuple with w=1.0 is a point");
  AddTest(&f, TestVector, "A tuple with w=0 is a vector");
  AddTest(&f, TestPointFactory, "point() creates tuples with w=1");
  AddTest(&f, TestVectorFactory, "vector() creates tuples with w=0");
  AddTest(&f, TestTupleAdd, "Adding two tuples");
  AddTest(&f, TestPointSub, "Subtracting two points");
  AddTest(&f, TestTupleSub, "Subtracting a vector from a point");
  AddTest(&f, TestVectorSub, "Subtracting a vector from a vector");
  AddTest(&f, TestZeroSub, "Subtracting a vector from the zero vector");
  AddTest(&f, TestNegation, "Negating a tuple");
  AddTest(&f, TestMultiply, "Multiplying a tuple by a scalar");
  AddTest(&f, TestMultiplyFrac, "Multiplying a tuple by a fraction");
  AddTest(&f, TestDivide, "Dividing a tuple by a scalar");
  AddTest(&f, TestMagnitude, "Computing the Magnitude of a tuple");
  AddTest(&f, TestNormalize, "Normalizing Vectors");
  AddTest(&f, TestDot, "The dot product of two tuples");
  AddTest(&f, TestCross, "The cross product of two vectors");
  AddTest(&f, TestCannon, "Cannon Simulation");
  AddTest(&f, TestColorAdd, "Adding Colors");
  AddTest(&f, TestColorSub, "Sutracting Colors");
  AddTest(&f, TestColorMult, "Multuplying Color by a Scalar");
  AddTest(&f, TestColorProd, "Multuplying Color by a Color");
  AddFeature(f);
}
