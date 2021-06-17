#include <math.h>
#include "../base/tuple.h"
#include "../utils/consts.h"
#include "test.h"

void TestPoint() {
  Tuple t = {4.3, -4.2, 3.1, 1.0};
  Equal(t.x, 4.3);
  Equal(t.y, -4.2);
  Equal(t.z, 3.1);
  Equal(t.w, 1.0);
  True(isPoint(&t));
  False(isVector(&t));
}

void TestVector() {
  Tuple t = {4.3, -4.2, 3.1, 0.0};
  Equal(t.x, 4.3);
  Equal(t.y, -4.2);
  Equal(t.z, 3.1);
  Equal(t.w, 0.0);
  False(isPoint(&t));
  True(isVector(&t));
}

void TestPointFactory() {
  Tuple *t = point(4, -4, 3);
  Equal(t->x, 4);
  Equal(t->y, -4);
  Equal(t->z, 3);
  Equal(t->w, 1);
  True(isPoint(t));
  False(isVector(t));
}

void TestVectorFactory() {
  Tuple *t = vec(4, -4, 3);
  Equal(t->x, 4);
  Equal(t->y, -4);
  Equal(t->z, 3);
  Equal(t->w, 0);
  False(isPoint(t));
  True(isVector(t));
}

void TestTupleAdd() {
  Tuple *t1 = point(3, -2, 5);
  Tuple *t2 = vec(-2, 3, 1);
  add(t1, t2);
  Equal(t1->x, 1);
  Equal(t1->y, 1);
  Equal(t1->z, 6);
  Equal(t1->w, 1);
}

void TestPointSub() {
  Tuple *t1 = point(3, 2, 1);
  Tuple *t2 = point(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isVector(t1));
}

void TestTupleSub() {
  Tuple *t1 = point(3, 2, 1);
  Tuple *t2 = vec(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isPoint(t1));
}

void TestVectorSub() {
  Tuple *t1 = vec(3, 2, 1);
  Tuple *t2 = vec(5, 6, 7);
  sub(t1, t2);
  Equal(t1->x, -2);
  Equal(t1->y, -4);
  Equal(t1->z, -6);
  True(isVector(t1));
}

void TestZeroSub() {
  Tuple *t1 = vec(0, 0, 0);
  Tuple *t2 = vec(1, -2, 3);
  sub(t1, t2);
  Equal(t1->x, -1);
  Equal(t1->y, 2);
  Equal(t1->z, -3);
  True(isVector(t1));
}

void TestNegation() {
  Tuple t1 = {1, -2, 3, -4};
  neg(&t1);
  Tuple a = {-1, 2, -3, 4};
  TupleEqual(&t1, &a);
}

void TestMultiply() {
  Tuple t1 = {1, -2, 3, -4};
  mult(&t1, 3.5);
  Tuple a = {3.5, -7, 10.5, -14};
  TupleEqual(&t1, &a);
}

void TestMultiplyFrac() {
  Tuple t1 = {1, -2, 3, -4};
  mult(&t1, 0.5);
  Tuple a = {0.5, -1, 1.5, -2};
  TupleEqual(&t1, &a);
}

void TestDivide() {
  Tuple t1 = {1, -2, 3, -4};
  divide(&t1, 2.0);
  Tuple a = {0.5, -1, 1.5, -2};
  TupleEqual(&t1, &a);
}

void TestMagnitude() {
    Tuple tuples[5] = {
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
    Tuple tuples[2] = {
        {4,0,0},
        {1,2,3},
    };
    Tuple res[2] = {
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
  Tuple *a = vec(1,2,3);
  Tuple *b = vec(2,3,4);
  Equal(dot(a,b), 20);
  free(a);free(b);
}

void TestCross() {
  Tuple *a = vec(1,2,3);
  Tuple *b = vec(2,3,4);
  Tuple *c1 = cross(a,b);
  Tuple *c2 = cross(b,a);
  Tuple d = {-1, 2, -1};
  Tuple e = {1, -2, 1};
  TupleEqual(c1, &d);
  TupleEqual(c2, &e);
  free(a);free(b);free(c1);free(c2);
}

void __tick(Tuple *p_pos, Tuple *p_vel, Tuple *env_g, Tuple *env_wind) {
  add(p_pos, p_vel);
  add(p_vel, env_g);
  add(p_vel, env_wind);
}

void TestCannon() {
  Tuple *p_pos, *p_vel, *env_g, *env_wind;
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
  Tuple *c1 = vec(0.9, 0.6, 0.75);
  Tuple *c2 = vec(0.7, 0.1, 0.25);
  Tuple color = {1.6, 0.7, 1.0};
  add(c1,c2);
  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestColorSub() {
  Tuple *c1 = vec(0.9, 0.6, 0.75);
  Tuple *c2 = vec(0.7, 0.1, 0.25);
  Tuple color = {0.2, 0.5, 0.5};
  sub(c1,c2);
  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestColorMult() {
  Tuple *c1 = vec(0.2, 0.3, 0.4);
  Tuple color = {0.4, 0.6, 0.8};
  mult(c1, 2.0);
  TupleEqual(c1, &color);

  free(c1);
}

void TestColorProd() {
  Tuple *c1 = vec(1.0, 0.2, 0.4);
  Tuple *c2 = vec(0.9, 1.0, 0.1);
  Tuple color = {0.9, 0.2, 0.04};
  prod(c1,c2);

  TupleEqual(c1, &color);

  free(c1); free(c2);
}

void TestReflect() {
  Tuple vs[2] = {
    {1, -1, 0},
    {0, -1, 0}
  };
  Tuple ns[2] = {
    {0, 1, 0},
    {sqrt(2)/2, sqrt(2)/2,0}
  };
  Tuple exps[2] = {
    {1,1,0},
    {1,0,0}
  };

  for (unsigned int i = 0; i < 2; i++) {
    reflect(&vs[i], &ns[i]);
    TupleEqual(&vs[i], &exps[i]);
  }
}

void TestTupleFeature() {
  Feature f = {"Tuples, Vectors, and Points"};
  AddTest(&f, TestPoint, "A Tuple with w=1.0 is a point");
  AddTest(&f, TestVector, "A Tuple with w=0 is a vector");
  AddTest(&f, TestPointFactory, "point() creates tuples with w=1");
  AddTest(&f, TestVectorFactory, "vector() creates tuples with w=0");
  AddTest(&f, TestTupleAdd, "Adding two tuples");
  AddTest(&f, TestPointSub, "Subtracting two points");
  AddTest(&f, TestTupleSub, "Subtracting a vector from a point");
  AddTest(&f, TestVectorSub, "Subtracting a vector from a vector");
  AddTest(&f, TestZeroSub, "Subtracting a vector from the zero vector");
  AddTest(&f, TestNegation, "Negating a Tuple");
  AddTest(&f, TestMultiply, "Multiplying a Tuple by a scalar");
  AddTest(&f, TestMultiplyFrac, "Multiplying a Tuple by a fraction");
  AddTest(&f, TestDivide, "Dividing a Tuple by a scalar");
  AddTest(&f, TestMagnitude, "Computing the Magnitude of a Tuple");
  AddTest(&f, TestNormalize, "Normalizing Vectors");
  AddTest(&f, TestDot, "The dot product of two tuples");
  AddTest(&f, TestCross, "The cross product of two vectors");
  AddTest(&f, TestCannon, "Cannon Simulation");
  AddTest(&f, TestColorAdd, "Adding Colors");
  AddTest(&f, TestColorSub, "Sutracting Colors");
  AddTest(&f, TestColorMult, "Multuplying Color by a Scalar");
  AddTest(&f, TestColorProd, "Multuplying Color by a Color");
  AddTest(&f, TestReflect, "Reflecting Vectors");
  AddFeature(f);
}
