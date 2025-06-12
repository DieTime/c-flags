/*
 * SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include "string-view.h"

StringView sv_from_string(const char *string)
{
    if (string == NULL)
        return (StringView){.data = NULL, .size = 0};

    return (StringView){.data = string, .size = strlen(string)};
}

StringView sv_slice_left(StringView sv, size_t count)
{
    if (count == 0 || sv.size < count)
        return sv_from_string(NULL);

    return (StringView){.data = sv.data, .size = count};
}

StringView sv_chop_left(StringView sv, size_t count)
{
    if (sv.size <= count)
        return sv_from_string(NULL);

    return (StringView){.data = sv.data + count, .size = sv.size - count};
}

bool sv_equal(StringView a, StringView b)
{
    if (a.size != b.size)
        return false;

    return !memcmp(a.data, b.data, a.size);
}

bool sv_starts_with(StringView a, StringView b)
{
    if (b.data == NULL)
        return false;

    return sv_equal(sv_slice_left(a, b.size), b);
}

bool sv_contains(StringView a, StringView b)
{
    return sv_index_of(a, b) != -1;
}

int sv_index_of(StringView a, StringView b)
{
    StringView null = sv_from_string(NULL);

    if (sv_equal(a, null) || sv_equal(b, null))
        return -1;

    char *pos = strstr(a.data, b.data);
    if (pos == NULL)
        return -1;

    return (int) (pos - a.data);
}
