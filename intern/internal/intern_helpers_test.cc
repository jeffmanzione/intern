extern "C" {
#include "intern/internal/intern_helpers.h"
}

#include <gtest/gtest.h>

namespace {

TEST(ComputeNearestPow2Gte, Zero) {
  // Handle 0-case bale-out.
  EXPECT_EQ(0, compute_nearest_pow2_gte(0));
}

TEST(ComputeNearestPow2Gte, PowsOf2) {
  EXPECT_EQ(1u << 0, compute_nearest_pow2_gte(1u << 0));
  EXPECT_EQ(1u << 1, compute_nearest_pow2_gte(1u << 1));
  EXPECT_EQ(1u << 2, compute_nearest_pow2_gte(1u << 2));
  EXPECT_EQ(1u << 3, compute_nearest_pow2_gte(1u << 3));
  EXPECT_EQ(1u << 4, compute_nearest_pow2_gte(1u << 4));
  EXPECT_EQ(1u << 5, compute_nearest_pow2_gte(1u << 5));
  EXPECT_EQ(1u << 6, compute_nearest_pow2_gte(1u << 6));
  EXPECT_EQ(1u << 7, compute_nearest_pow2_gte(1u << 7));
  EXPECT_EQ(1u << 8, compute_nearest_pow2_gte(1u << 8));
  EXPECT_EQ(1u << 9, compute_nearest_pow2_gte(1u << 9));
  EXPECT_EQ(1u << 10, compute_nearest_pow2_gte(1u << 10));
  EXPECT_EQ(1u << 11, compute_nearest_pow2_gte(1u << 11));
  EXPECT_EQ(1u << 12, compute_nearest_pow2_gte(1u << 12));
  EXPECT_EQ(1u << 13, compute_nearest_pow2_gte(1u << 13));
  EXPECT_EQ(1u << 14, compute_nearest_pow2_gte(1u << 14));
  EXPECT_EQ(1u << 15, compute_nearest_pow2_gte(1u << 15));
  EXPECT_EQ(1u << 16, compute_nearest_pow2_gte(1u << 16));
  EXPECT_EQ(1u << 17, compute_nearest_pow2_gte(1u << 17));
  EXPECT_EQ(1u << 18, compute_nearest_pow2_gte(1u << 18));
  EXPECT_EQ(1u << 19, compute_nearest_pow2_gte(1u << 19));
  EXPECT_EQ(1u << 20, compute_nearest_pow2_gte(1u << 20));
  EXPECT_EQ(1u << 21, compute_nearest_pow2_gte(1u << 21));
  EXPECT_EQ(1u << 22, compute_nearest_pow2_gte(1u << 22));
  EXPECT_EQ(1u << 23, compute_nearest_pow2_gte(1u << 23));
  EXPECT_EQ(1u << 24, compute_nearest_pow2_gte(1u << 24));
  EXPECT_EQ(1u << 25, compute_nearest_pow2_gte(1u << 25));
  EXPECT_EQ(1u << 26, compute_nearest_pow2_gte(1u << 26));
  EXPECT_EQ(1u << 27, compute_nearest_pow2_gte(1u << 27));
  EXPECT_EQ(1u << 28, compute_nearest_pow2_gte(1u << 28));
  EXPECT_EQ(1u << 29, compute_nearest_pow2_gte(1u << 29));
  EXPECT_EQ(1u << 30, compute_nearest_pow2_gte(1u << 30));
  EXPECT_EQ(1u << 31, compute_nearest_pow2_gte(1u << 31));
}

TEST(ComputeNearestPow2Gte, PowsOf2Minus1) {
  EXPECT_EQ(1u << 2, compute_nearest_pow2_gte((1u << 2) - 1));
  EXPECT_EQ(1u << 3, compute_nearest_pow2_gte((1u << 3) - 1));
  EXPECT_EQ(1u << 4, compute_nearest_pow2_gte((1u << 4) - 1));
  EXPECT_EQ(1u << 5, compute_nearest_pow2_gte((1u << 5) - 1));
  EXPECT_EQ(1u << 6, compute_nearest_pow2_gte((1u << 6) - 1));
  EXPECT_EQ(1u << 7, compute_nearest_pow2_gte((1u << 7) - 1));
  EXPECT_EQ(1u << 8, compute_nearest_pow2_gte((1u << 8) - 1));
  EXPECT_EQ(1u << 9, compute_nearest_pow2_gte((1u << 9) - 1));
  EXPECT_EQ(1u << 10, compute_nearest_pow2_gte((1u << 10) - 1));
  EXPECT_EQ(1u << 11, compute_nearest_pow2_gte((1u << 11) - 1));
  EXPECT_EQ(1u << 12, compute_nearest_pow2_gte((1u << 12) - 1));
  EXPECT_EQ(1u << 13, compute_nearest_pow2_gte((1u << 13) - 1));
  EXPECT_EQ(1u << 14, compute_nearest_pow2_gte((1u << 14) - 1));
  EXPECT_EQ(1u << 15, compute_nearest_pow2_gte((1u << 15) - 1));
  EXPECT_EQ(1u << 16, compute_nearest_pow2_gte((1u << 16) - 1));
  EXPECT_EQ(1u << 17, compute_nearest_pow2_gte((1u << 17) - 1));
  EXPECT_EQ(1u << 18, compute_nearest_pow2_gte((1u << 18) - 1));
  EXPECT_EQ(1u << 19, compute_nearest_pow2_gte((1u << 19) - 1));
  EXPECT_EQ(1u << 20, compute_nearest_pow2_gte((1u << 20) - 1));
  EXPECT_EQ(1u << 21, compute_nearest_pow2_gte((1u << 21) - 1));
  EXPECT_EQ(1u << 22, compute_nearest_pow2_gte((1u << 22) - 1));
  EXPECT_EQ(1u << 23, compute_nearest_pow2_gte((1u << 23) - 1));
  EXPECT_EQ(1u << 24, compute_nearest_pow2_gte((1u << 24) - 1));
  EXPECT_EQ(1u << 25, compute_nearest_pow2_gte((1u << 25) - 1));
  EXPECT_EQ(1u << 26, compute_nearest_pow2_gte((1u << 26) - 1));
  EXPECT_EQ(1u << 27, compute_nearest_pow2_gte((1u << 27) - 1));
  EXPECT_EQ(1u << 28, compute_nearest_pow2_gte((1u << 28) - 1));
  EXPECT_EQ(1u << 29, compute_nearest_pow2_gte((1u << 29) - 1));
  EXPECT_EQ(1u << 30, compute_nearest_pow2_gte((1u << 30) - 1));
  EXPECT_EQ(1u << 31, compute_nearest_pow2_gte((1u << 31) - 1));
}

TEST(ComputeNearestPow2Gte, PowsOf2Plus1) {
  EXPECT_EQ((1u << 1), compute_nearest_pow2_gte((1u << 0) + 1));
  EXPECT_EQ((1u << 2), compute_nearest_pow2_gte((1u << 1) + 1));
  EXPECT_EQ((1u << 3), compute_nearest_pow2_gte((1u << 2) + 1));
  EXPECT_EQ((1u << 4), compute_nearest_pow2_gte((1u << 3) + 1));
  EXPECT_EQ((1u << 5), compute_nearest_pow2_gte((1u << 4) + 1));
  EXPECT_EQ((1u << 6), compute_nearest_pow2_gte((1u << 5) + 1));
  EXPECT_EQ((1u << 7), compute_nearest_pow2_gte((1u << 6) + 1));
  EXPECT_EQ((1u << 8), compute_nearest_pow2_gte((1u << 7) + 1));
  EXPECT_EQ((1u << 9), compute_nearest_pow2_gte((1u << 8) + 1));
  EXPECT_EQ((1u << 10), compute_nearest_pow2_gte((1u << 9) + 1));
  EXPECT_EQ((1u << 11), compute_nearest_pow2_gte((1u << 10) + 1));
  EXPECT_EQ((1u << 12), compute_nearest_pow2_gte((1u << 11) + 1));
  EXPECT_EQ((1u << 13), compute_nearest_pow2_gte((1u << 12) + 1));
  EXPECT_EQ((1u << 14), compute_nearest_pow2_gte((1u << 13) + 1));
  EXPECT_EQ((1u << 15), compute_nearest_pow2_gte((1u << 14) + 1));
  EXPECT_EQ((1u << 16), compute_nearest_pow2_gte((1u << 15) + 1));
  EXPECT_EQ((1u << 17), compute_nearest_pow2_gte((1u << 16) + 1));
  EXPECT_EQ((1u << 18), compute_nearest_pow2_gte((1u << 17) + 1));
  EXPECT_EQ((1u << 19), compute_nearest_pow2_gte((1u << 18) + 1));
  EXPECT_EQ((1u << 20), compute_nearest_pow2_gte((1u << 19) + 1));
  EXPECT_EQ((1u << 21), compute_nearest_pow2_gte((1u << 20) + 1));
  EXPECT_EQ((1u << 22), compute_nearest_pow2_gte((1u << 21) + 1));
  EXPECT_EQ((1u << 23), compute_nearest_pow2_gte((1u << 22) + 1));
  EXPECT_EQ((1u << 24), compute_nearest_pow2_gte((1u << 23) + 1));
  EXPECT_EQ((1u << 25), compute_nearest_pow2_gte((1u << 24) + 1));
  EXPECT_EQ((1u << 26), compute_nearest_pow2_gte((1u << 25) + 1));
  EXPECT_EQ((1u << 27), compute_nearest_pow2_gte((1u << 26) + 1));
  EXPECT_EQ((1u << 28), compute_nearest_pow2_gte((1u << 27) + 1));
  EXPECT_EQ((1u << 29), compute_nearest_pow2_gte((1u << 28) + 1));
  EXPECT_EQ((1u << 30), compute_nearest_pow2_gte((1u << 29) + 1));
  EXPECT_EQ((1u << 31), compute_nearest_pow2_gte((1u << 30) + 1));
}

}  // namespace