#include "intern/internal/intern_helpers.h"

uint32_t compute_nearest_pow2_gte(uint32_t num) {
  if (num == 0) {
    return 0;
  }
  num--;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;
  return num + 1;
}