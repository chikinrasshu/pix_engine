#include "mapping.h"

r32 map_raw(r32 v, r32 src_a, r32 src_b, r32 dst_a, r32 dst_b) {
  r32 slope = 1.0 * (dst_b - dst_a) / (src_b - src_a);
  return dst_a + slope * (v - src_a);
}

r32 map_1d(r32 v, v2 src, v2 dst) {
  return map_raw(v, src.x, src.y, dst.x, dst.y);
}

v2 map_2d(v2 p, v2 src_a, v2 src_b, v2 dst_a, v2 dst_b) {
  return (v2){map_raw(p.x, src_a.x, src_b.x, dst_a.x, dst_b.x),
              map_raw(p.y, src_a.y, src_b.y, dst_a.y, dst_b.y)};
}
