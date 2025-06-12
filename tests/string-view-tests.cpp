/*
 * SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <gtest/gtest.h>
#include "string-view.h"

TEST(StringViewTest, sv_from_string)
{
    StringView sv = sv_from_string(nullptr);
    EXPECT_TRUE(sv.data == nullptr);
    EXPECT_EQ(sv.size, 0);

    sv = sv_from_string("hello");
    EXPECT_STREQ(sv.data, "hello");
    EXPECT_EQ(sv.size, strlen("hello"));

    sv = sv_from_string("");
    EXPECT_STREQ(sv.data, "");
    EXPECT_EQ(sv.size, 0);
}

TEST(StringViewTest, sv_slice_left)
{
    StringView null = sv_from_string(nullptr);
    StringView sv = null;

    StringView sv_part = sv_slice_left(sv, 0);
    EXPECT_TRUE(sv_equal(sv_part, null));

    sv_part = sv_slice_left(sv, 1);
    EXPECT_TRUE(sv_equal(sv_part, null));

    sv = sv_from_string("hello");

    sv_part = sv_slice_left(sv, 1);
    EXPECT_TRUE(sv_equal(sv_part, sv_from_string("h")));

    sv_part = sv_slice_left(sv, sv.size);
    EXPECT_TRUE(sv_equal(sv_part, sv));

    sv_part = sv_slice_left(sv, sv.size + 1);
    EXPECT_TRUE(sv_equal(sv_part, null));
}

TEST(StringViewTest, sv_chop_left)
{
    StringView null = sv_from_string(nullptr);
    StringView sv = null;

    StringView sv_chopped = sv_chop_left(sv, 1);
    EXPECT_TRUE(sv_equal(sv_chopped, null));

    sv_chopped = sv_chop_left(sv, 0);
    EXPECT_TRUE(sv_equal(sv_chopped, null));

    sv = sv_from_string("hello");

    sv_chopped = sv_chop_left(sv, 1);
    EXPECT_TRUE(sv_equal(sv_chopped, sv_from_string("ello")));

    sv_chopped = sv_chop_left(sv, 0);
    EXPECT_TRUE(sv_equal(sv_chopped, sv));

    sv_chopped = sv_chop_left(sv, sv.size);
    EXPECT_TRUE(sv_equal(sv_chopped, null));
}

TEST(StringViewTest, sv_equal)
{
    StringView null = sv_from_string(nullptr);
    StringView sv = null;

    EXPECT_TRUE(sv_equal(sv, null));

    sv = sv_from_string("hello");

    EXPECT_TRUE(sv_equal(sv, sv_from_string("hello")));
    EXPECT_FALSE(sv_equal(sv, sv_from_string("helloo")));
    EXPECT_FALSE(sv_equal(sv, sv_from_string("he")));
    EXPECT_FALSE(sv_equal(sv, sv_from_string("")));
    EXPECT_FALSE(sv_equal(sv, null));
}

TEST(StringViewTest, sv_starts_with)
{
    StringView null = sv_from_string(nullptr);
    StringView sv = sv_from_string("hello");

    EXPECT_FALSE(sv_starts_with(null, null));
    EXPECT_FALSE(sv_starts_with(sv, null));
    EXPECT_FALSE(sv_starts_with(sv, sv_from_string("ello")));

    EXPECT_TRUE(sv_starts_with(sv, sv_from_string("")));
    EXPECT_TRUE(sv_starts_with(sv, sv_from_string("h")));
    EXPECT_TRUE(sv_starts_with(sv, sv_from_string("he")));
    EXPECT_TRUE(sv_starts_with(sv, sv));

    sv = sv_from_string("");

    EXPECT_TRUE(sv_starts_with(sv, sv));
}

TEST(StringViewTest, sv_contains)
{
    StringView null = sv_from_string(nullptr);
    StringView empty = sv_from_string("");
    StringView sv = sv_from_string("hello");

    EXPECT_FALSE(sv_contains(null, null));
    EXPECT_FALSE(sv_contains(sv, null));
    EXPECT_FALSE(sv_contains(sv, empty));
    EXPECT_FALSE(sv_contains(empty, empty));

    EXPECT_TRUE(sv_contains(sv, sv_from_string("h")));
    EXPECT_TRUE(sv_contains(sv, sv_from_string("he")));
    EXPECT_TRUE(sv_contains(sv, sv));
    EXPECT_TRUE(sv_contains(sv, sv_from_string("ello")));
    EXPECT_TRUE(sv_contains(sv, sv_from_string("l")));
}

TEST(StringViewTest, sv_index_of)
{
    StringView null = sv_from_string(nullptr);
    StringView empty = sv_from_string("");
    StringView sv = sv_from_string("hello");

    EXPECT_EQ(sv_index_of(null, null), -1);
    EXPECT_EQ(sv_index_of(empty, null), -1);
    EXPECT_EQ(sv_index_of(null, empty), -1);
    EXPECT_EQ(sv_index_of(sv, sv_from_string("")), -1);
    EXPECT_EQ(sv_index_of(sv, sv_from_string("a")), -1);

    EXPECT_EQ(sv_index_of(sv, sv_from_string("h")), 0);
    EXPECT_EQ(sv_index_of(sv, sv_from_string("o")), 4);
}
