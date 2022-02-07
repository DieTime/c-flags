#include <c-flags.h>
#include <gtest/gtest.h>

TEST(CFlagsTestsDefault, Positive)
{
    int *int_value = c_flag_int("int", "i", nullptr, 0);
    int8_t *int8_value = c_flag_int8("int8", "i8", nullptr, 0);
    int16_t *int16_value = c_flag_int16("int16", "i16", nullptr, 0);
    int32_t *int32_value = c_flag_int32("int32", "i32", nullptr, 0);
    int64_t *int64_value = c_flag_int64("int64", "i64", nullptr, 0);
    ssize_t *ssize_t_value = c_flag_ssize_t("ssize", "ss", nullptr, 0);

    unsigned *unsigned_value = c_flag_unsigned("unsigned", "u", nullptr, 0);
    uint8_t *uint8_value = c_flag_uint8("uint8", "u8", nullptr, 0);
    uint16_t *uint16_value = c_flag_uint16("uint16", "u16", nullptr, 0);
    uint32_t *uint32_value = c_flag_uint32("uint32", "u32", nullptr, 0);
    uint64_t *uint64_value = c_flag_uint64("uint64", "u64", nullptr, 0);
    size_t *size_t_value = c_flag_size_t("size", "s", nullptr, 0);

    bool *bool_value = c_flag_bool("bool", "b", nullptr, false);
    char **string_value = c_flag_string("string", "str", nullptr, "hello");

    const char *argv_raw[] = {"app"};
    char **argv = (char **) argv_raw;
    int argc = 1;

    c_flags_parse(&argc, &argv, false);

    EXPECT_EQ(*int_value, 0);
    EXPECT_EQ(*int8_value, 0);
    EXPECT_EQ(*int16_value, 0);
    EXPECT_EQ(*int32_value, 0);
    EXPECT_EQ(*int64_value, 0);
    EXPECT_EQ(*ssize_t_value, 0);

    EXPECT_EQ(*unsigned_value, 0);
    EXPECT_EQ(*uint8_value, 0);
    EXPECT_EQ(*uint16_value, 0);
    EXPECT_EQ(*uint32_value, 0);
    EXPECT_EQ(*uint64_value, 0);
    EXPECT_EQ(*size_t_value, 0);

    EXPECT_EQ(*bool_value, false);
    EXPECT_STREQ(*string_value, "hello");

    EXPECT_EQ(argc, 0);
}
