/*
 * SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <c-flags.h>
#include <gtest/gtest.h>

TEST(CFlagsTestsLongNameWithEqual, Positive)
{
    int *int_value = c_flag_int("int", "i", nullptr, 0);
    int8_t *int8_value = c_flag_int8("int8", "i8", nullptr, 0);
    int16_t *int16_value = c_flag_int16("int16", "i16", nullptr, 0);
    int32_t *int32_value = c_flag_int32("int32", "i32", nullptr, 0);
    int64_t *int64_value = c_flag_int64("int64", "i64", nullptr, 0);

    unsigned *unsigned_value = c_flag_unsigned("unsigned", "u", nullptr, 0);
    uint8_t *uint8_value = c_flag_uint8("uint8", "u8", nullptr, 0);
    uint16_t *uint16_value = c_flag_uint16("uint16", "u16", nullptr, 0);
    uint32_t *uint32_value = c_flag_uint32("uint32", "u32", nullptr, 0);
    uint64_t *uint64_value = c_flag_uint64("uint64", "u64", nullptr, 0);
    size_t *size_t_value = c_flag_size_t("size", "s", nullptr, 0);

    bool *bool_value = c_flag_bool("bool", "b", nullptr, false);
    char **string_value = c_flag_string("string", "str", nullptr, "hello");

    float *float_value = c_flag_float("float", "flt", nullptr, 1.0);
    double *double_value = c_flag_double("double", "dbl", nullptr, 1.0);

    // clang-format off
    const char *argv_raw[] = {"app", "--int=1",
                                     "--int8=1",
                                     "--int16=1",
                                     "--int32=1",
                                     "--int64=1",
                                     "--unsigned=1",
                                     "--uint8=1",
                                     "--uint16=1",
                                     "--uint32=1",
                                     "--uint64=1",
                                     "--size=1",
                                     "--bool",
                                     "--string=hello-world",
                                     "--float=1.0",
                                     "--double=1.0"};
    // clang-format on
    char **argv = (char **) argv_raw;
    int argc = (int) (sizeof(argv_raw) / sizeof(argv_raw[0]));

    c_flags_parse(&argc, &argv, false);

    EXPECT_EQ(*int_value, 1);
    EXPECT_EQ(*int8_value, 1);
    EXPECT_EQ(*int16_value, 1);
    EXPECT_EQ(*int32_value, 1);
    EXPECT_EQ(*int64_value, 1);

    EXPECT_EQ(*unsigned_value, 1U);
    EXPECT_EQ(*uint8_value, 1U);
    EXPECT_EQ(*uint16_value, 1U);
    EXPECT_EQ(*uint32_value, 1U);
    EXPECT_EQ(*uint64_value, 1U);
    EXPECT_EQ(*size_t_value, 1U);

    EXPECT_EQ(*bool_value, true);
    EXPECT_STREQ(*string_value, "hello-world");

    EXPECT_EQ(*float_value, 1.0);
    EXPECT_EQ(*double_value, 1.0);

    EXPECT_EQ(argc, 0);
}

// clang-format off
#define DECLARE_NEGATIVE_TEST(postfix, incorrect_value, test_number)                               \
    TEST(CFlagsTestsLongNameWithEqual, NegativeBadTest##postfix##test_number)                      \
    {                                                                                              \
        void *bad = (void *) c_flag_##postfix("value" #postfix #test_number, nullptr, nullptr, 0); \
        (void) bad;                                                                                \
                                                                                                   \
        const char *argv_raw[] = {"app", "--value" #postfix #test_number "=" incorrect_value };    \
        char **argv = (char **) argv_raw;                                                          \
        int argc = (int) (sizeof(argv_raw) / sizeof(argv_raw[0]));                                 \
                                                                                                   \
        EXPECT_EXIT(c_flags_parse(&argc, &argv, false), testing::ExitedWithCode(1), ".*");         \
    }
// clang-format off


DECLARE_NEGATIVE_TEST(int, "a", 1)
DECLARE_NEGATIVE_TEST(int, "9223372036854775808", 2)
DECLARE_NEGATIVE_TEST(int, "-9223372036854775809", 3)

DECLARE_NEGATIVE_TEST(int8, "a", 1)
DECLARE_NEGATIVE_TEST(int8, "128", 2)
DECLARE_NEGATIVE_TEST(int8, "-129", 3)

DECLARE_NEGATIVE_TEST(int16, "a", 1)
DECLARE_NEGATIVE_TEST(int16, "32768", 2)
DECLARE_NEGATIVE_TEST(int16, "-32769", 3)

DECLARE_NEGATIVE_TEST(int32, "a", 1)
DECLARE_NEGATIVE_TEST(int32, "2147483648", 2)
DECLARE_NEGATIVE_TEST(int32, "-2147483649", 3)

DECLARE_NEGATIVE_TEST(int64, "a", 1)
DECLARE_NEGATIVE_TEST(int64, "9223372036854775808", 2)
DECLARE_NEGATIVE_TEST(int64, "-9223372036854775809", 3)

DECLARE_NEGATIVE_TEST(unsigned, "a", 1)
DECLARE_NEGATIVE_TEST(unsigned, "-1", 2)
DECLARE_NEGATIVE_TEST(unsigned, "18446744073709551616", 3)

DECLARE_NEGATIVE_TEST(uint8, "a", 1)
DECLARE_NEGATIVE_TEST(uint8, "-1", 2)
DECLARE_NEGATIVE_TEST(uint8, "256", 3)

DECLARE_NEGATIVE_TEST(uint16, "a", 1)
DECLARE_NEGATIVE_TEST(uint16, "-1", 2)
DECLARE_NEGATIVE_TEST(uint16, "65536", 3)

DECLARE_NEGATIVE_TEST(uint32, "a", 1)
DECLARE_NEGATIVE_TEST(uint32, "-1", 2)
DECLARE_NEGATIVE_TEST(uint32, "4294967296", 3)

DECLARE_NEGATIVE_TEST(uint64, "a", 1)
DECLARE_NEGATIVE_TEST(uint64, "-1", 2)
DECLARE_NEGATIVE_TEST(uint64, "18446744073709551616", 3)

DECLARE_NEGATIVE_TEST(size_t, "a", 1)
DECLARE_NEGATIVE_TEST(size_t, "-1", 2)
DECLARE_NEGATIVE_TEST(size_t, "18446744073709551616", 3)

DECLARE_NEGATIVE_TEST(float, "a", 1)
DECLARE_NEGATIVE_TEST(float, "1.a1", 2)
DECLARE_NEGATIVE_TEST(float, "-1.a1", 3)

DECLARE_NEGATIVE_TEST(double, "a", 1)
DECLARE_NEGATIVE_TEST(double, "1.a1", 2)
DECLARE_NEGATIVE_TEST(double, "-1.a1", 3)
