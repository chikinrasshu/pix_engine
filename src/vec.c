#include "vec.h"
#include <math.h>

/*********/
/* Vec 2 */
/*********/

v2 v2_add(v2 a, v2 b) { return (v2){a.x + b.x, a.y + b.y}; }
v2 v2_sub(v2 a, v2 b) { return (v2){a.x - b.x, a.y - b.y}; }
v2 v2_hadamard(v2 a, v2 b) { return (v2){a.x * b.x, a.y * b.y}; }
v2 v2_mul(v2 v, r32 s) { return (v2){v.x * s, v.y * s}; }
v2 v2_div(v2 v, r32 d) { return (v2){v.x / d, v.y / d}; }
r32 v2_dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
r32 v2_len_sq(v2 v) { return (v.x * v.x + v.y * v.y); }
r32 v2_len(v2 v) { return sqrtf(v2_len_sq(v)); }
r32 v2_dist_sq(v2 a, v2 b) { return (v2_len_sq(v2_sub(b, a))); }
r32 v2_dist(v2 a, v2 b) { return sqrtf(v2_dist_sq(a, b)); }

/*********/
/* Vec 3 */
/*********/

v3 v3_add(v3 a, v3 b) { return (v3){a.x + b.x, a.y + b.y, a.z + b.z}; }
v3 v3_sub(v3 a, v3 b) { return (v3){a.x - b.x, a.y - b.y, a.z - b.z}; }
v3 v3_hadamard(v3 a, v3 b) { return (v3){a.x * b.x, a.y * b.y, a.z * b.z}; }
v3 v3_cross(v3 a, v3 b) {
  return (v3){a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x,
              a.x * b.y - a.y * b.x};
}
v3 v3_mul(v3 v, r32 s) { return (v3){v.x * s, v.y * s, v.z * s}; }
v3 v3_div(v3 v, r32 d) { return (v3){v.x / d, v.y / d, v.z / d}; }
r32 v3_dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
r32 v3_len_sq(v3 v) { return (v.x * v.x + v.y * v.y + v.z * v.z); }
r32 v3_len(v3 v) { return sqrtf(v3_len_sq(v)); }
r32 v3_dist_sq(v3 a, v3 b) { return (v3_len_sq(v3_sub(b, a))); }
r32 v3_dist(v3 a, v3 b) { return sqrtf(v3_dist_sq(a, b)); }

/*********/
/* Vec 4 */
/*********/

v4 v4_add(v4 a, v4 b) {
  return (v4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
v4 v4_sub(v4 a, v4 b) {
  return (v4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
v4 v4_hadamard(v4 a, v4 b) {
  return (v4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}
v4 v4_mul(v4 v, r32 s) { return (v4){v.x * s, v.y * s, v.z * s, v.w * s}; }
v4 v4_div(v4 v, r32 d) { return (v4){v.x / d, v.y / d, v.z / d, v.w / d}; }
r32 v4_dot(v4 a, v4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
r32 v4_len_sq(v4 v) { return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }
r32 v4_len(v4 v) { return sqrtf(v4_len_sq(v)); }
r32 v4_dist_sq(v4 a, v4 b) { return (v4_len_sq(v4_sub(b, a))); }
r32 v4_dist(v4 a, v4 b) { return sqrtf(v4_dist_sq(a, b)); }

/*************/
/* Vec 2 Int */
/*************/

v2i v2i_add(v2i a, v2i b) { return (v2i){a.x + b.x, a.y + b.y}; }
v2i v2i_sub(v2i a, v2i b) { return (v2i){a.x - b.x, a.y - b.y}; }
v2i v2i_hadamard(v2i a, v2i b) { return (v2i){a.x * b.x, a.y * b.y}; }
v2i v2i_mul(v2i v, s32 s) { return (v2i){v.x * s, v.y * s}; }
v2i v2i_div(v2i v, s32 d) { return (v2i){v.x / d, v.y / d}; }
s32 v2i_dot(v2i a, v2i b) { return a.x * b.x + a.y * b.y; }
s32 v2i_len_sq(v2i v) { return (v.x * v.x + v.y * v.y); }
s32 v2i_len(v2i v) { return (s32)sqrtf(v2i_len_sq(v)); }
s32 v2i_dist_sq(v2i a, v2i b) { return (v2i_len_sq(v2i_sub(b, a))); }
s32 v2i_dist(v2i a, v2i b) { return (s32)sqrtf(v2i_dist_sq(a, b)); }

/*************/
/* Vec 3 Int */
/*************/

v3i v3i_add(v3i a, v3i b) { return (v3i){a.x + b.x, a.y + b.y, a.z + b.z}; }
v3i v3i_sub(v3i a, v3i b) { return (v3i){a.x - b.x, a.y - b.y, a.z - b.z}; }
v3i v3i_hadamard(v3i a, v3i b) {
  return (v3i){a.x * b.x, a.y * b.y, a.z * b.z};
}
v3i v3i_cross(v3i a, v3i b) {
  return (v3i){a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x,
               a.x * b.y - a.y * b.x};
}
v3i v3i_mul(v3i v, s32 s) { return (v3i){v.x * s, v.y * s, v.z * s}; }
v3i v3i_div(v3i v, s32 d) { return (v3i){v.x / d, v.y / d, v.z / d}; }
s32 v3i_dot(v3i a, v3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
s32 v3i_len_sq(v3i v) { return (v.x * v.x + v.y * v.y + v.z * v.z); }
s32 v3i_len(v3i v) { return (s32)sqrtf(v3i_len_sq(v)); }
s32 v3i_dist_sq(v3i a, v3i b) { return (v3i_len_sq(v3i_sub(b, a))); }
s32 v3i_dist(v3i a, v3i b) { return (s32)sqrtf(v3i_dist_sq(a, b)); }

/*************/
/* Vec 4 Int */
/*************/

v4i v4i_add(v4i a, v4i b) {
  return (v4i){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
v4i v4i_sub(v4i a, v4i b) {
  return (v4i){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
v4i v4i_hadamard(v4i a, v4i b) {
  return (v4i){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}
v4i v4i_mul(v4i v, s32 s) { return (v4i){v.x * s, v.y * s, v.z * s, v.w * s}; }
v4i v4i_div(v4i v, s32 d) { return (v4i){v.x / d, v.y / d, v.z / d, v.w / d}; }
s32 v4i_dot(v4i a, v4i b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
s32 v4i_len_sq(v4i v) {
  return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}
s32 v4i_len(v4i v) { return (s32)sqrtf(v4i_len_sq(v)); }
s32 v4i_dist_sq(v4i a, v4i b) { return (v4i_len_sq(v4i_sub(b, a))); }
s32 v4i_dist(v4i a, v4i b) { return (s32)sqrtf(v4i_dist_sq(a, b)); }
