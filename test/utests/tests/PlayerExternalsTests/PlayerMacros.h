#pragma once
#include <gtest/gtest.h>

#define GTEST_TEST_NO_THROW_(statement, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::TrueWithString gtest_msg{}) { \
    try { \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    GTEST_TEST_NO_THROW_CATCH_STD_EXCEPTION_() \
    catch (...) { \
      gtest_msg.value = "it throws."; \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__): \
      fail(("Expected: " #statement " doesn't throw an exception.\n" \
            "  Actual: " + gtest_msg.value).c_str())

#define MY_EXPECT_NO_THROW(statement)                 \
    do {                                              \
        try {                                        \
            statement;                               \
        } catch (const std::exception& e) {         \
            FAIL() << "Expected no throw, but threw std::exception: " << e.what(); \
        } catch (...) {                              \
            FAIL() << "Expected no throw, but threw unknown exception."; \
        }                                            \
    } while (0)

