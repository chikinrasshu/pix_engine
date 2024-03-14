#pragma once

#include "common.h"

/*********/
/* Float */
/*********/

typedef struct v2 {
  r32 x, y;
} v2;

v2 v2_add(v2 a, v2 b);
v2 v2_sub(v2 a, v2 b);
v2 v2_hadamard(v2 a, v2 b);
v2 v2_mul(v2 v, r32 s);
v2 v2_div(v2 v, r32 d);
r32 v2_dot(v2 a, v2 b);
r32 v2_len_sq(v2 v);
r32 v2_len(v2 v);
r32 v2_dist_sq(v2 a, v2 b);
r32 v2_dist(v2 a, v2 b);

typedef struct v3 {
  r32 x, y, z;
} v3;

v3 v3_add(v3 a, v3 b);
v3 v3_sub(v3 a, v3 b);
v3 v3_hadamard(v3 a, v3 b);
v3 v3_cross(v3 a, v3 b);
v3 v3_mul(v3 v, r32 s);
v3 v3_div(v3 v, r32 d);
r32 v3_dot(v3 a, v3 b);
r32 v3_len_sq(v3 v);
r32 v3_len(v3 v);
r32 v3_dist_sq(v3 a, v3 b);
r32 v3_dist(v3 a, v3 b);

typedef struct v4 {
  r32 x, y, z, w;
} v4;

v4 v4_add(v4 a, v4 b);
v4 v4_sub(v4 a, v4 b);
v4 v4_hadamard(v4 a, v4 b);
v4 v4_mul(v4 a, r32 b);
v4 v4_div(v4 a, r32 b);
r32 v4_dot(v4 a, v4 b);
r32 v4_len_sq(v4 v);
r32 v4_len(v4 v);
r32 v4_dist_sq(v4 a, v4 b);
r32 v4_dist(v4 a, v4 b);

/*******/
/* Int */
/*******/

typedef struct v2i {
  s32 x, y;
} v2i;

v2i v2i_add(v2i a, v2i b);
v2i v2i_sub(v2i a, v2i b);
v2i v2i_hadamard(v2i a, v2i b);
v2i v2i_mul(v2i v, s32 s);
v2i v2i_div(v2i v, s32 d);
s32 v2i_dot(v2i a, v2i b);
s32 v2i_len_sq(v2i v);
s32 v2i_len(v2i v);
s32 v2i_dist_sq(v2i a, v2i b);
s32 v2i_dist(v2i a, v2i b);

typedef struct v3i {
  s32 x, y, z;
} v3i;

v3i v3i_add(v3i a, v3i b);
v3i v3i_sub(v3i a, v3i b);
v3i v3i_hadamard(v3i a, v3i b);
v3i v3i_cross(v3i a, v3i b);
v3i v3i_mul(v3i v, s32 s);
v3i v3i_div(v3i v, s32 d);
s32 v3i_dot(v3i a, v3i b);
s32 v3i_len_sq(v3i v);
s32 v3i_len(v3i v);
s32 v3i_dist_sq(v3i a, v3i b);
s32 v3i_dist(v3i a, v3i b);

typedef struct v4i {
  r32 x, y, z, w;
} v4i;

v4i v4i_add(v4i a, v4i b);
v4i v4i_sub(v4i a, v4i b);
v4i v4i_hadamard(v4i a, v4i b);
v4i v4i_mul(v4i a, s32 b);
v4i v4i_div(v4i a, s32 b);
s32 v4i_dot(v4i a, v4i b);
s32 v4i_len_sq(v4i v);
s32 v4i_len(v4i v);
s32 v4i_dist_sq(v4i a, v4i b);
s32 v4i_dist(v4i a, v4i b);
