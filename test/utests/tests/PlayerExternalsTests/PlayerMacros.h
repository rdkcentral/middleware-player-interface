#pragma once
#include <gtest/gtest.h>



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

