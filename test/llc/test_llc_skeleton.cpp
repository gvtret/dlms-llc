#include "dlms/llc/llc_skeleton.hpp"

#include <gtest/gtest.h>

TEST(LlcSkeletonTest, LibraryLinks)
{
  EXPECT_TRUE(dlms::llc::LlcSkeletonLinked());
}
