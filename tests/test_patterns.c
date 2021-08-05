#include "../base/pattern.h"
#include "test.h"

void TestStripePatternStripeAtY() {
    Tuple *black = vec(0,0,0);
    Tuple *white = vec(1,1,1);
    Pattern *pattern = stripe_pattern(white, black);
    Tuple p1 = {0,0,0,1}, p2 = {0,1,0,1}, p3 = {0,2,0,1};
    TupleEqual(stripe_at(pattern, &p1), white);
    TupleEqual(stripe_at(pattern, &p2), white);
    TupleEqual(stripe_at(pattern, &p3), white);

    free_pattern(pattern);
}

void TestStripePatternStripeAtZ() {
    Tuple *black = vec(0,0,0);
    Tuple *white = vec(1,1,1);
    Pattern *pattern = stripe_pattern(white, black);
    Tuple p1 = {0,0,0,1}, p2 = {0,0,1,1}, p3 = {0,0,2,1};
    TupleEqual(stripe_at(pattern, &p1), white);
    TupleEqual(stripe_at(pattern, &p2), white);
    TupleEqual(stripe_at(pattern, &p3), white);

    free_pattern(pattern);
}

void TestStripePatternStripeAtX() {
    Tuple *black = vec(0,0,0);
    Tuple *white = vec(1,1,1);
    Pattern *pattern = stripe_pattern(white, black);
    Tuple p1 = {0,0,0,1},
          p2 = {0.9,0,0,1},
          p3 = {1,0,0,1},
          p4 = {-0.1,0,0,1},
          p5 = {-1,0,0,1},
          p6 = {-1.1,0,0,1};
    TupleEqual(stripe_at(pattern, &p1), white);
    TupleEqual(stripe_at(pattern, &p2), white);
    TupleEqual(stripe_at(pattern, &p3), black);
    TupleEqual(stripe_at(pattern, &p4), black);
    TupleEqual(stripe_at(pattern, &p5), black);
    TupleEqual(stripe_at(pattern, &p6), white);

    free_pattern(pattern);
}


void TestPatternsFeature() {
  Feature f = {"Patterns"};
  AddTest(&f, TestStripePatternStripeAtY, "A stripe pattern is constant in y");
  AddTest(&f, TestStripePatternStripeAtZ, "A stripe pattern is constant in z");
  AddTest(&f, TestStripePatternStripeAtX, "A stripe pattern alternates in x");
  AddFeature(f);
}