#include "../rtc.h"

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

void TestStripeObjectTransform() {
    Solid *s = sphere();
    set_transform(s, scaling(2,2,2));
    Pattern *p = stripe_pattern(vec(1,1,1), vec(0,0,0));

    Tuple *pt = point(1.5, 0, 0);
    Tuple *c = stripe_at_object(p, s, pt);
    Tuple white = {1,1,1};

    TupleEqual(c, &white);
    free(c); free(pt); free_solid(s); free_pattern(p);
}

void TestStripePatternTransform() {
    Solid *s = sphere();
    Pattern *p = stripe_pattern(vec(1,1,1), vec(0,0,0));
    set_pattern_transform(p, scaling(2,2,2));

    Tuple *pt = point(1.5, 0, 0);
    Tuple *c = stripe_at_object(p, s, pt);
    Tuple white = {1,1,1};

    TupleEqual(c, &white);
    free(c); free(pt); free_solid(s); free_pattern(p);
}

void TestStripeMultiTransform() {
    Solid *s = sphere();
    set_transform(s, scaling(2,2,2));
    Pattern *p = stripe_pattern(vec(1,1,1), vec(0,0,0));
    set_pattern_transform(p, translation(0.5,0,0));

    Tuple *pt = point(2.5, 0, 0);
    Tuple *c = stripe_at_object(p, s, pt);
    Tuple white = {1,1,1};

    TupleEqual(c, &white);
    free(c); free(pt); free_solid(s); free_pattern(p);
}

void TestPatternsFeature() {
  Feature f = {"Patterns"};
  AddTest(&f, TestStripePatternStripeAtY, "A stripe pattern is constant in y");
  AddTest(&f, TestStripePatternStripeAtZ, "A stripe pattern is constant in z");
  AddTest(&f, TestStripePatternStripeAtX, "A stripe pattern alternates in x");
  AddTest(&f, TestStripeObjectTransform, "Stripes with an object transformation");
  AddTest(&f, TestStripePatternTransform, "Stripes with a pattern transformation");
  AddTest(&f, TestStripeMultiTransform, "Stripes with a pattern and object transformation");
  AddFeature(f);
}
