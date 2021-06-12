#pragma once


#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define CLAMP(min, x, max) (MIN(max, MAX(min, x)))
