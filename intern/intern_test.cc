extern "C" {
#include "intern/intern.h"
}

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using namespace testing;

DEFINE_INTERN_POOL(Int32InternPool, int32_t);

DEFINE_INTERN_POOL(StringInternPool, char);
IMPL_INTERN_POOL(StringInternPool, char);

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
#define FNV_32_PRIME (0x01000193)
#define FNV_1A_32_OFFSET (0x811C9DC5)

// Is true if any bytes in the provided 32-bit integer are equivalent to 0x00.
#define HAS_NULL(x)                                      \
  (((x & 0x000000FF) == 0) || ((x & 0x0000FF00) == 0) || \
   ((x & 0x00FF0000) == 0) || ((x & 0xFF000000) == 0))

uint32_t hash_string(const char *ptr, uint32_t size) {
  unsigned char *s = (unsigned char *)ptr;
  uint32_t hval = FNV_1A_32_OFFSET;
  for (uint32_t i = 0; i < size; ++i) {
    hval *= FNV_32_PRIME;
    hval ^= (uint32_t)*s++;
  }
  return hval;
}

int32_t compare_strings(const char *ptr1, uint32_t size1, const char *ptr2,
                        uint32_t size2) {
  if (ptr1 == ptr2) {
    return 0;
  }
  if (NULL == ptr1) {
    return -1;
  }
  if (NULL == ptr2) {
    return 1;
  }
  return memcmp(ptr1, ptr2, std::max(size1, size2));
}

class StringInternPoolTest : public Test {
 protected:
  StringInternPoolTest() {
    StringInternPool_init(&intern_pool, /*threadsafe=*/false, hash_string,
                          compare_strings);
  }
  ~StringInternPoolTest() { StringInternPool_finalize(&intern_pool); }
  StringInternPool intern_pool;
};

TEST_F(StringInternPoolTest, Init) {
  // Just test setup/teardown.
}

TEST_F(StringInternPoolTest, InternPool) {
  // Insert
  const char *str = StringInternPool_intern(&intern_pool, "cat", sizeof("cat"));

  // Verify it returns same string
  ASSERT_THAT(str, NotNull());
  ASSERT_EQ(str, StringInternPool_intern(&intern_pool, str, sizeof("cat")));
}

TEST_F(StringInternPoolTest, InternPoolN) {
  // Insert
  const char *cat = StringInternPool_intern(&intern_pool, "cat", sizeof("cat"));
  const char *in = StringInternPool_intern(&intern_pool, "in", sizeof("in"));
  const char *the = StringInternPool_intern(&intern_pool, "the", sizeof("the"));
  const char *hat = StringInternPool_intern(&intern_pool, "hat", sizeof("hat"));

  // Verify it returns same string
  ASSERT_THAT(cat, NotNull());
  ASSERT_THAT(in, NotNull());
  ASSERT_THAT(the, NotNull());
  ASSERT_THAT(hat, NotNull());

  // Verify uniqueness of equivalent strings
  ASSERT_EQ(cat, StringInternPool_intern(&intern_pool, "cat", sizeof("cat")));
  ASSERT_EQ(cat, StringInternPool_intern(&intern_pool, cat, sizeof("cat")));
  ASSERT_EQ(in, StringInternPool_intern(&intern_pool, "in", sizeof("in")));
  ASSERT_EQ(in, StringInternPool_intern(&intern_pool, in, sizeof("in")));
  ASSERT_EQ(the, StringInternPool_intern(&intern_pool, "the", sizeof("the")));
  ASSERT_EQ(the, StringInternPool_intern(&intern_pool, the, sizeof("the")));
  ASSERT_EQ(hat, StringInternPool_intern(&intern_pool, "hat", sizeof("hat")));
  ASSERT_EQ(hat, StringInternPool_intern(&intern_pool, hat, sizeof("hat")));
}

}  // namespace