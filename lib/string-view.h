/*
 * SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    const char *data;
    size_t size;
} StringView;

#define SVFMT     "%.*s"
#define SVARG(sv) (int) (sv).size, (sv).data

/**
 * Create string view from string
 *
 * @param string line to create
 * @return string view instance
 */
StringView sv_from_string(const char *string);

/**
 * Create string view from `count` characters of passed string view
 *
 * @param sv string view from which a new one is created
 * @param count number of characters
 * @return string view instance
 */
StringView sv_slice_left(StringView sv, size_t count);

/**
 * Create string view by indenting `count` characters to the left of passed string view
 *
 * @param sv string view from which a new one is created
 * @param count number of characters to indenting
 * @return string view instance
 */
StringView sv_chop_left(StringView sv, size_t count);

/**
 * Check if string view instances are equal
 *
 * @param a first string view instance
 * @param b second string view instance
 * @return true if equal, otherwise false
 */
bool sv_equal(StringView a, StringView b);

/**
 * Check if one string view starts with the second one
 *
 * @param a first string view instance
 * @param b second string view instance
 * @return true if `a` starts with `b`, otherwise false
 */
bool sv_starts_with(StringView a, StringView b);

/**
 * Check if one string view contains the second one
 *
 * @param a first string view instance
 * @param b second string view instance
 * @return true if `a` contains `b`, otherwise false
 */
bool sv_contains(StringView a, StringView b);

/**
 * Find index where the second string view starts in the first
 *
 * @param a first string view instance
 * @param b second string view instance
 * @return index in `a` string if `a` contains `b`, otherwise -1
 */
int sv_index_of(StringView a, StringView b);

#ifdef __cplusplus
}
#endif

#endif // STRING_VIEW_H
