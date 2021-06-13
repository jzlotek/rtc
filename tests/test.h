#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../utils/consts.h"
#include "../base/tuple.h"

#pragma once

typedef struct {
    void (*func)();
    char *description;
} Test;

typedef struct {
    char *description;
    int num_tests;
    int tests_length;
    Test *tests;
} Feature;

static Feature *features = NULL;
static int rtc_num_features;
static int rtc_features_length;
static int rtc_tests_passed;

void clean_feaures() {
    for (unsigned int i = 0; i < rtc_num_features; i++) {
        Feature f = features[i];
        free(f.tests);
    }
    free(features);
}

void AddFeature(Feature f) {
    if (features == NULL) {
        features = (Feature*)malloc(sizeof(Feature) * 2);
        rtc_features_length = 2;
        rtc_num_features = 0;
    }
    if (rtc_features_length == rtc_num_features) {
        features = (Feature*)realloc(features, sizeof(Feature) * 2 * rtc_features_length);
        rtc_features_length *= 2;
    }
    features[rtc_num_features] = f;
    rtc_num_features++;
}

void AddTest(Feature *f, void (*func)(), char *desc) {
    Test test = {func, desc};
    if (f->tests == NULL) {
        f->tests = (Test*)malloc(sizeof(Test) * 2);
        f->tests_length = 2;
        f->num_tests = 0;
    }
    if (f->tests_length == f->num_tests) {
        f->tests = (Test*)realloc(f->tests, sizeof(Test) * 2 * f->tests_length);
        f->tests_length *= 2;
    }

    f->tests[f->num_tests] = test;
    f->num_tests++;
}

static int rtc_statements_failed;
static char *rtc_statements[512];

int RunTests() {
    int total_tests_run = 0;
    printf("RTC Tests\n");

    for (unsigned int f = 0; f < rtc_num_features; f++) {
        Feature feature = features[f];
        printf("\tFeature: %s\n", feature.description);
        total_tests_run += feature.num_tests;
        for (unsigned int i = 0; i < feature.num_tests; i++) {
            Test t = feature.tests[i];
            rtc_statements_failed = 0;
            printf("\t\tTest: %s ", t.description);
            t.func();
            if (rtc_statements_failed == 0) {
                printf("%s\n", "✔️");
                rtc_tests_passed++;
            } else {
                printf("%s\n", "❌");
            }
            for (unsigned int j = 0; j < rtc_statements_failed; j++) {
                printf(rtc_statements[j]);
            }
        }
    }
    printf("Total: %d | %d ✔️ | %d ❌\n", total_tests_run, rtc_tests_passed, total_tests_run - rtc_tests_passed);
    return rtc_tests_passed == total_tests_run ? 0 : 1;
}

int TestMain() {
    int result = RunTests();
    clean_feaures();
    return result;
}

bool TupleEqual(Tuple *t1, Tuple *t2) {
    bool result = equal(t1, t2);
    if (!result) {
        char buf[512];
        sprintf(buf, "\t\t\t{%.6f, %.6f, %.6f, %.6f} == {%.6f, %.6f, %.6f, %.6f}\n",
                t1->x, t1->y, t1->z, t1->w, t2->x, t2->y, t2->z, t2->w);
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return result;
}

bool Equal(float a, float b) {
    bool result = fabs(a - b) < EPSILON;
    if (!result) {
        char buf[512];
        sprintf(buf, "\t\t\t%.10f == %.10f\n", a, b);
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return result;
}

bool LT(float a, float b) {
    bool result = a < b;
    if (!result) {
        char buf[256];
        sprintf(buf, "\t\t\t%.10f < %.10f\n", a, b);
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return result;
}

bool GT(float a, float b) {
    bool result = a > b;
    if (!result) {
        char buf[256];
        sprintf(buf, "\t\t\t%.10f > %.10f\n", a, b);
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return result;
}

bool True(bool a) {
    if (!a) {
        char buf[256];
        sprintf(buf, "\t\t\tfalse\n");
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return a;
}

bool False(bool a) {
    if (a) {
        char buf[256];
        sprintf(buf, "\t\t\ttrue\n");
        rtc_statements[rtc_statements_failed] = buf;
        rtc_statements_failed++;
    }
    return !a;
}
