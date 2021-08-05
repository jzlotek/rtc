#include "tuple.h"

#pragma once

typedef enum {
    Stripe,
} PatternType;

typedef struct {
    PatternType type;
    void *pattern_p;
    void (*free_func)(void *);
    Tuple *(*stripe_at_func)(void *pattern, const Tuple *point);
} Pattern;

typedef struct {
    Tuple *a;
    Tuple *b;
} StripePattern;

Pattern *stripe_pattern(Tuple *a, Tuple *b);
void free_pattern(Pattern *pattern);
Tuple *stripe_at(Pattern *pattern, const Tuple *point);
Pattern *pattern_container();

Pattern *pattern_conatainer() {
    Pattern *pattern = (Pattern*)malloc(sizeof(Pattern));
    return pattern;
}

void free_stripe_pattern(void *p) {
    StripePattern *pattern = (StripePattern*)p;
    free(pattern->a);
    free(pattern->b);
    free(pattern);
}

Tuple *stripe_at_helper(void *p, const Tuple *point) {
    StripePattern *pattern = (StripePattern*)p;
    if ((int)floor(point->x) % 2 == 0) {
        return pattern->a;
    }
    return pattern->b;
}

Pattern *stripe_pattern(Tuple *a, Tuple *b) {
    Pattern *p = pattern_conatainer();
    StripePattern *sp = (StripePattern*)malloc(sizeof(StripePattern));
    sp->a = a;
    sp->b = b;
    p->pattern_p = sp;
    p->free_func = free_stripe_pattern;
    p->stripe_at_func = stripe_at_helper;
    return p;
}

void free_pattern(Pattern *pattern) {
    pattern->free_func(pattern->pattern_p);
    free(pattern);
}


Tuple *stripe_at(Pattern *pattern, const Tuple *point) {
    return pattern->stripe_at_func(pattern->pattern_p, point);
}