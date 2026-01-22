#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "utility-c/utils/mutex.h"

TEST(MutexTest, LockUnlock)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      int iterations;
    } in;
    struct Want
    {
      int lock_result;
      int unlock_result;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"lock-unlock-once", {1}, {0, 0}},
    {"lock-unlock-twice", {2}, {0, 0}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    s_mutex_t mutex;

    for (int i = 0; i < tc.in.iterations; ++i)
    {
      // Act
      int got_lock = mutex_lock(&mutex);
      int got_unlock = mutex_unlock(&mutex);

      // Assert
      EXPECT_EQ(got_lock, tc.want.lock_result);
      EXPECT_EQ(got_unlock, tc.want.unlock_result);
    }
  }
}
