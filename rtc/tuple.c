#include "../rtc.h"

bool isPoint(Tuple *t) {
    return fabs(t->vals[3] - 1.0f) < EPSILON;
}

bool isVector(Tuple *t) {
    return fabs(t->vals[3]) < EPSILON;
}

void print_tuple(Tuple *t) {
    if (isPoint(t)) {
        printf("(%.6f, %6f, %.6f, %.6f)", t->vals[0], t->vals[1], t->vals[2], t->vals[3]);
    } else {
        printf("<%.6f, %6f, %.6f>", t->vals[0], t->vals[1], t->vals[2]);
    }
}

Tuple *copy_tuple(const Tuple *src) {
    Tuple *t = point(0,0,0);
    return copy_from(t, src);
}

Tuple *copy_from(Tuple *dest, const Tuple *src) {
    dest->vals[0] = src->vals[0];
    dest->vals[1] = src->vals[1];
    dest->vals[2] = src->vals[2];
    dest->vals[3] = src->vals[3];
    return dest;
}

bool equal(const Tuple *t1, const Tuple *t2) {
    __m128 res = _mm_sub_ps(t1->vec, t2->vec);

    return _mm_movemask_ps(_mm_cmplt_epi32(res, _mm_set1_ps(EPSILON))) != 0;
}

Tuple *point(float x, float y, float z) {
    Tuple *r = (Tuple*)malloc(sizeof(Tuple));
    r->vals[0] = x;
    r->vals[1] = y;
    r->vals[2] = z;
    r->vals[3] = 1.0;
    return r;
}

Tuple *vec(float x, float y, float z) {
    Tuple *r = (Tuple*)malloc(sizeof(Tuple));
    r->vals[0] = x;
    r->vals[1] = y;
    r->vals[2] = z;
    r->vals[3] = 0.0;
    return r;
}

Tuple *add(Tuple *t1, const Tuple *t2) {
    t1->vec = _mm_add_ps(t1->vec, t2->vec);
    return t1;
}

Tuple *sub(Tuple *t1, const Tuple *t2) {
    __m128 res = _mm_sub_ps(t1->vec, t2->vec);
    t1->vec = res;
    return t1;
}

Tuple *neg(Tuple *t1) {
    __m128 res = _mm_mul_ps(t1->vec, _mm_set1_ps(-1));
    t1->vec = res;
    return t1;
}

Tuple *mult(Tuple *t1, float c) {
    __m128 res = _mm_mul_ps(t1->vec, _mm_set1_ps(c));
    t1->vec = res;
    return t1;
}

Tuple *prod(Tuple *t1, const Tuple *t2) {
    __m128 res = _mm_mul_ps(t1->vec, t2->vec);
    t1->vec = res;
    return t1;
}

Tuple *divide(Tuple *t1, float c) {
    __m128 res = _mm_div_ps(t1->vec, _mm_set1_ps(c));
    t1->vec = res;
    return t1;
}

float magnitude(const Tuple *t){
    __m128 res = _mm_mul_ps(t->vec, t->vec);
    res = _mm_hadd_ps(res, res);
    res = _mm_hadd_ps(res, res);
    return sqrt(res[0]);
}

Tuple *norm(Tuple *t) {
    return divide(t, magnitude(t));
}

float dot(const Tuple *t1, const Tuple *t2) {
    __m128 res = _mm_mul_ps(t1->vec, t2->vec);
    res = _mm_hadd_ps(res, res);
    res = _mm_hadd_ps(res, res);
    return res[0];
}

Tuple *cross(const Tuple *a, const Tuple *b) {
    if (a->vals[3] != 0 || b->vals[3] != 0) {
        perror("Cannot call cross on points (w must be equal to 0)");
        exit(1);
    }
    __m128 tmp0 = _mm_shuffle_ps(a->vec, a->vec, _MM_SHUFFLE(3,0,2,1));
    __m128 tmp1 = _mm_shuffle_ps(b->vec, b->vec, _MM_SHUFFLE(3,1,0,2));
    __m128 tmp2 = _mm_mul_ps(tmp0, b->vec);
    __m128 tmp3 = _mm_mul_ps(tmp0, tmp1);
    __m128 tmp4 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3,0,2,1));

    Tuple *res = vec(0,0,0);
    res->vec = _mm_sub_ps(tmp3, tmp4);
    return res;
}

Tuple *reflect(Tuple *a, const Tuple *b) {
    Tuple *b_cpy = copy_tuple(b);
    Tuple *res =  sub(a, mult(b_cpy, 2 * dot(a, b)));
    free(b_cpy);
    return res;
}
