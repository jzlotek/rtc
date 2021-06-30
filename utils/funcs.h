#pragma once

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define CLAMP(min, x, max) (MIN(max, MAX(min, x)))
#define String char*

#define TEMPLATE_ARRAY(TYPE)\
typedef struct {\
    unsigned int length;\
    unsigned int num_elems;\
    TYPE *data;\
} TYPE##Array;\
\
TYPE##Array *TYPE##_array() {\
    TYPE##Array *arr = (TYPE##Array*)malloc(sizeof(TYPE##Array));\
    arr->length = 0;\
    arr->num_elems = 2;\
    arr->data = (TYPE*)malloc(sizeof(TYPE) * 2);\
    return arr;\
}\
\
void free_##TYPE##_array(TYPE##Array *arr) {\
    free(arr->data);\
    free(arr);\
}\
\
TYPE get_##TYPE##_array(TYPE##Array *arr, unsigned int idx) {\
    return arr->data[idx];\
}\
\
void TYPE##_arr_add(TYPE##Array *arr, TYPE v) {\
     if (arr->length == arr->num_elems) {\
         arr->data = (TYPE*)realloc(arr->data, sizeof(TYPE) * 2 * arr->num_elems);\
         arr->num_elems *= 2;\
     }\
     arr->data[arr->length] = v;\
     arr->length++;\
}

TEMPLATE_ARRAY(int);
TEMPLATE_ARRAY(float);
TEMPLATE_ARRAY(String);

void clean_String_array(StringArray *a) {
    for (unsigned int i = 0; i < a->length; i++) {
        free(a->data[i]);
    }
}