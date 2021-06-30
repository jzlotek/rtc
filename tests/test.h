#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../utils/consts.h"
#include "../utils/funcs.h"
#include "../base/tuple.h"

#pragma once

const int RTC_BUFLEN = 256;
typedef struct {
    void (*func)();
    char *description;
} Test;

TEMPLATE_ARRAY(Test);

typedef struct {
    char *description;
    TestArray *tests;
} Feature;

TEMPLATE_ARRAY(Feature);

static FeatureArray *features = NULL;

void AddFeature(Feature f) {
    if (features == NULL) {
        features = Feature_array();
    }
    Feature_arr_add(features, f);
}

void AddTest(Feature *f, void (*func)(), char *desc) {
    Test test = {func, desc};
    if (f->tests == NULL) {
        f->tests = Test_array();
    }
    Test_arr_add(f->tests, test);
}

static StringArray *rtc_statements;

int RunTests() {
    int total_tests_run = 0;
    int rtc_tests_passed = 0;
    printf("RTC Tests\n");

    for (unsigned int f = 0; f < features->length; f++) {
        Feature feature = get_Feature_array(features, f);
        printf("\tFeature: %s\n", feature.description);
        total_tests_run += feature.tests->length;
        for (unsigned int i = 0; i < feature.tests->length; i++) {
            rtc_statements = String_array();
            Test t = get_Test_array(feature.tests, i);
            printf("\t\tTest: %s ", t.description);
            t.func();
            if (rtc_statements->length == 0) {
                printf("%s\n", "✔️");
                rtc_tests_passed++;
            } else {
                printf("%s\n", "❌");
            }
            for (unsigned int j = 0; j < rtc_statements->length; j++) {
                printf(get_String_array(rtc_statements, j));
            }
            clean_String_array(rtc_statements);
            free_String_array(rtc_statements);
        }
    }
    printf("Total: %d | %d ✔️ | %d ❌\n", total_tests_run, rtc_tests_passed, total_tests_run - rtc_tests_passed);
    return rtc_tests_passed == total_tests_run ? 0 : 1;
}

int TestMain() {
    int result = RunTests();
    for (unsigned int i = 0; i < features->length; i++) {
        free_Test_array(get_Feature_array(features, i).tests);
    }
    free_Feature_array(features);
    return result;
}

bool TupleEqual(Tuple *t1, Tuple *t2) {
    bool result = equal(t1, t2);
    if (!result) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN * 2);
        sprintf(buf, "\t\t\t Got: {%.6f, %.6f, %.6f, %.6f}, Expected: {%.6f, %.6f, %.6f, %.6f}\n",
                t1->x, t1->y, t1->z, t1->w, t2->x, t2->y, t2->z, t2->w);
        String_arr_add(rtc_statements, buf);
    }
    return result;
}

bool Equal(float a, float b) {
    bool result = fabs(a - b) < EPSILON;
    if (!result) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN);
        sprintf(buf, "\t\t\t%.10f == %.10f\n", a, b);
        String_arr_add(rtc_statements, buf);
    }
    return result;
}

bool LT(float a, float b) {
    bool result = a < b;
    if (!result) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN);
        sprintf(buf, "\t\t\t%.10f < %.10f\n", a, b);
        String_arr_add(rtc_statements, buf);
    }
    return result;
}

bool GT(float a, float b) {
    bool result = a > b;
    if (!result) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN);
        sprintf(buf, "\t\t\t%.10f > %.10f\n", a, b);
        String_arr_add(rtc_statements, buf);
    }
    return result;
}

bool True(bool a) {
    if (!a) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN);
        sprintf(buf, "\t\t\tfalse\n");
        String_arr_add(rtc_statements, buf);
    }
    return a;
}

bool False(bool a) {
    if (a) {
        char *buf = (char*)malloc(sizeof(char) * RTC_BUFLEN);
        sprintf(buf, "\t\t\ttrue\n");
        String_arr_add(rtc_statements, buf);
    }
    return !a;
}
