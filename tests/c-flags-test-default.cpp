#include <c-flags.h>
#include <gtest/gtest.h>

// clang-format off
#define DECLARE_DEFAULT_TEST(type, postfix, default_value)                                     \
    TEST(CFlagsTestsDefault, Positive##postfix)                                                \
    {                                                                                          \
        type *value_##postfix = c_flag_##postfix(#postfix, nullptr, nullptr, (default_value)); \
                                                                                               \
        const char *argv_raw[] = {"app"};                                                      \
        char **argv = (char **) argv_raw;                                                      \
        int argc = 1;                                                                          \
                                                                                               \
        c_flags_parse(&argc, &argv, false);                                                    \
                                                                                               \
        EXPECT_EQ(*value_##postfix, (default_value));                                          \
    }
// clang-format on

DECLARE_DEFAULT_TEST(int, int, 0)
DECLARE_DEFAULT_TEST(int8_t, int8, 0)
DECLARE_DEFAULT_TEST(int16_t, int16, 0)
DECLARE_DEFAULT_TEST(int32_t, int32, 0)
DECLARE_DEFAULT_TEST(int64_t, int64, 0)
DECLARE_DEFAULT_TEST(unsigned, unsigned, 0)
DECLARE_DEFAULT_TEST(uint8_t, uint8, 0)
DECLARE_DEFAULT_TEST(uint16_t, uint16, 0)
DECLARE_DEFAULT_TEST(uint32_t, uint32, 0)
DECLARE_DEFAULT_TEST(uint64_t, uint64, 0)
DECLARE_DEFAULT_TEST(size_t, size_t, 0)
DECLARE_DEFAULT_TEST(bool, bool, false)
DECLARE_DEFAULT_TEST(char *, string, "hello")
DECLARE_DEFAULT_TEST(float, float, 0.0)
DECLARE_DEFAULT_TEST(double, double, 0.0)
