#include <c-flags.h>
#include <gtest/gtest.h>

TEST(CFlagsTestsNoValue, LongNameNoEqual)
{
    int *int_value = c_flag_int("test1", "t1", nullptr, 0);
    const char *argv_raw[] = {"app", "--test1"};

    (void) int_value;

    char **argv = (char **) argv_raw;
    int argc = (int) (sizeof(argv_raw) / sizeof(argv_raw[0]));

    testing::internal::CaptureStdout();

    EXPECT_EXIT({
        c_flags_parse(&argc, &argv, false);
    }, ::testing::ExitedWithCode(1), ".*");

    EXPECT_NE(testing::internal::GetCapturedStdout().find("no value for flag"), std::string::npos);
}

TEST(CFlagsTestsNoValue, LongName)
{
    int *int_value = c_flag_int("test2", "t2", nullptr, 0);
    const char *argv_raw[] = {"app", "--test2="};

    (void) int_value;

    char **argv = (char **) argv_raw;
    int argc = (int) (sizeof(argv_raw) / sizeof(argv_raw[0]));

    testing::internal::CaptureStdout();

    EXPECT_EXIT({
        c_flags_parse(&argc, &argv, false);
    }, ::testing::ExitedWithCode(1), ".*");

    EXPECT_NE(testing::internal::GetCapturedStdout().find("no value for flag"), std::string::npos);
}

TEST(CFlagsTestsNoValue, ShortName)
{
    int *int_value = c_flag_int("test3", "t3", nullptr, 0);
    const char *argv_raw[] = {"app", "-t3"};

    (void) int_value;

    char **argv = (char **) argv_raw;
    int argc = (int) (sizeof(argv_raw) / sizeof(argv_raw[0]));

    testing::internal::CaptureStdout();

    EXPECT_EXIT({
        c_flags_parse(&argc, &argv, false);
    }, ::testing::ExitedWithCode(1), ".*");

    EXPECT_NE(testing::internal::GetCapturedStdout().find("no value for flag"), std::string::npos);
}
