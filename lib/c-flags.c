/*
 * SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c-flags.h"
#include "string-view.h"

#ifndef C_FLAGS_CAPACITY
#define C_FLAGS_CAPACITY 64
#endif

typedef enum {
    C_FLAG_INT,
    C_FLAG_INT_8,
    C_FLAG_INT_16,
    C_FLAG_INT_32,
    C_FLAG_INT_64,
    C_FLAG_UNSIGNED,
    C_FLAG_UINT_8,
    C_FLAG_UINT_16,
    C_FLAG_UINT_32,
    C_FLAG_UINT_64,
    C_FLAG_SIZE_T,
    C_FLAG_BOOL,
    C_FLAG_STRING,
    C_FLAG_FLOAT,
    C_FLAG_DOUBLE,
} CFlagType;

typedef struct
{
    CFlagType type;
    const char *long_name;
    const char *short_name;
    const char *desc;
    uintmax_t default_data;
    uintmax_t data;
} CFlag;

static CFlag flags[C_FLAGS_CAPACITY] = {0};
static size_t flags_size = 0;

static char *c_flags_appname_message = NULL;
static char *c_flags_pos_args_desc = NULL;
static char *c_flags_description_message = NULL;

#define C_FLAG_DATA_AS_PTR(flag, ptr_type)         ((ptr_type *) (&((flag)->data)))
#define C_FLAG_DEFAULT_DATA_AS_PTR(flag, ptr_type) ((ptr_type *) (&((flag)->default_data)))

// clang-format off
#define C_FLAG_FILL(flag, _type, _long_name, _short_name, _desc) \
    {                                                            \
        (flag)->type = (_type);                                  \
        (flag)->long_name = (_long_name);                        \
        (flag)->short_name = (_short_name);                      \
        (flag)->desc = (_desc);                                  \
    }

#define DECLARE_C_FLAG_IMPL(type, ptr_type, postfix)                                        \
    ptr_type *c_flag_##postfix(const char *long_name,                                       \
                               const char *short_name,                                      \
                               const char *desc,                                            \
                               const ptr_type default_val)                                  \
    {                                                                                       \
        assert(flags_size < C_FLAGS_CAPACITY && "exceeding the maximum number of flags, "   \
                                                "please define C_FLAGS_CAPACITY according " \
                                                "to your needs when compile");              \
        assert(long_name != NULL && "the long name is required and cannot be NULL");        \
        assert(flag_names_unique(long_name, short_name) && "flag names must be unique");    \
                                                                                            \
        CFlag *flag = &flags[flags_size++];                                                 \
                                                                                            \
        C_FLAG_FILL(flag, type, long_name, short_name, desc)                                \
        *C_FLAG_DEFAULT_DATA_AS_PTR(flag, ptr_type) = (ptr_type) default_val;               \
        *C_FLAG_DATA_AS_PTR(flag, ptr_type) = (ptr_type) default_val;                       \
                                                                                            \
        return C_FLAG_DATA_AS_PTR(flag, ptr_type);                                          \
    }

#define C_FLAG_LOAD_SIGNED_VALUE(flag, is_flag_long, ptr_type, value, usage_on_error)              \
{                                                                                                  \
    char *end_ptr;                                                                                 \
    errno = 0;                                                                                     \
                                                                                                   \
    long long number = strtoll(value, &end_ptr, 10);                                               \
    bool value_fully_parsed = (size_t) (end_ptr - (value)) == strlen(value);                       \
                                                                                                   \
    /*
     * Getting signed number limits
     *  - min: b'10...0
     *  - max: b'01...1
     */                                                                                            \
    ptr_type max = 1;                                                                              \
    ptr_type min = (max) << (sizeof(ptr_type) * 8 - 1);                                            \
    (max) = ~(min);                                                                                \
                                                                                                   \
    if (errno != 0 || !value_fully_parsed || number < (min) || number > (max)) {                   \
        printf("ERROR: invalid value %s for " #ptr_type " flag %s%s\n",                            \
               (value),                                                                            \
               (is_flag_long) ? "--" : "-",                                                        \
               (is_flag_long) ? (flag)->long_name : (flag)->short_name);                           \
                                                                                                   \
        if (usage_on_error)                                                                        \
            c_flags_usage();                                                                       \
                                                                                                   \
        exit(1);                                                                                   \
    }                                                                                              \
                                                                                                   \
    *C_FLAG_DATA_AS_PTR(flag, ptr_type) = (ptr_type) number;                                       \
}

#define C_FLAG_LOAD_UNSIGNED_VALUE(flag, is_flag_long, ptr_type, value, usage_on_error)            \
{                                                                                                  \
    char *end_ptr;                                                                                 \
    errno = 0;                                                                                     \
                                                                                                   \
    unsigned long long number = strtoull(value, &end_ptr, 10);                                     \
    bool value_fully_parsed = (size_t) (end_ptr - (value)) == strlen(value);                       \
                                                                                                   \
    /*
     * Getting unsigned number limits
     *  - max: b'1...1
     */                                                                                            \
    ptr_type max = 0;                                                                              \
    (max) = ~(max);                                                                                \
                                                                                                   \
    if (errno != 0 || (value)[0] == '-' || !value_fully_parsed || number > (max)) {                \
        printf("ERROR: invalid value %s for " #ptr_type " flag %s%s\n",                            \
               (value),                                                                            \
               (is_flag_long) ? "--" : "-",                                                        \
               (is_flag_long) ? (flag)->long_name : (flag)->short_name);                           \
                                                                                                   \
        if (usage_on_error)                                                                        \
            c_flags_usage();                                                                       \
                                                                                                   \
        exit(1);                                                                                   \
    }                                                                                              \
                                                                                                   \
    *C_FLAG_DATA_AS_PTR(flag, ptr_type) = (ptr_type) number;                                       \
}

#define C_FLAG_LOAD_FLOATING_VALUE(flag, is_flag_long, ptr_type, value, strtox_fun, usage_on_error)\
{                                                                                                  \
    char *end_ptr;                                                                                 \
    errno = 0;                                                                                     \
                                                                                                   \
    ptr_type number = strtox_fun(value, &end_ptr);                                                 \
    bool value_fully_parsed = (size_t) (end_ptr - (value)) == strlen(value);                       \
                                                                                                   \
    if (errno != 0 || !value_fully_parsed) {                                                       \
        printf("ERROR: invalid value %s for " #ptr_type " flag %s%s\n",                            \
               (value),                                                                            \
               (is_flag_long) ? "--" : "-",                                                        \
               (is_flag_long) ? (flag)->long_name : (flag)->short_name);                           \
                                                                                                   \
        if (usage_on_error)                                                                        \
            c_flags_usage();                                                                       \
                                                                                                   \
        exit(1);                                                                                   \
    }                                                                                              \
                                                                                                   \
    *C_FLAG_DATA_AS_PTR(flag, ptr_type) = number;                                                  \
}
// clang-format on

static bool flag_names_unique(const char *long_name, const char *short_name)
{
    for (size_t i = 0; i < flags_size; i++) {
        CFlag *flag = &flags[i];

        if (!strcmp(long_name, flag->long_name))
            return false;

        if (short_name && flag->short_name && !strcmp(short_name, flag->short_name))
            return false;
    }

    return true;
}

DECLARE_C_FLAG_IMPL(C_FLAG_INT, int, int)
DECLARE_C_FLAG_IMPL(C_FLAG_INT_8, int8_t, int8)
DECLARE_C_FLAG_IMPL(C_FLAG_INT_16, int16_t, int16)
DECLARE_C_FLAG_IMPL(C_FLAG_INT_32, int32_t, int32)
DECLARE_C_FLAG_IMPL(C_FLAG_INT_64, int64_t, int64)
DECLARE_C_FLAG_IMPL(C_FLAG_UNSIGNED, unsigned, unsigned)
DECLARE_C_FLAG_IMPL(C_FLAG_UINT_8, uint8_t, uint8)
DECLARE_C_FLAG_IMPL(C_FLAG_UINT_16, uint16_t, uint16)
DECLARE_C_FLAG_IMPL(C_FLAG_UINT_32, uint32_t, uint32)
DECLARE_C_FLAG_IMPL(C_FLAG_UINT_64, uint64_t, uint64)
DECLARE_C_FLAG_IMPL(C_FLAG_SIZE_T, size_t, size_t)
DECLARE_C_FLAG_IMPL(C_FLAG_BOOL, bool, bool)
DECLARE_C_FLAG_IMPL(C_FLAG_STRING, char *, string)
DECLARE_C_FLAG_IMPL(C_FLAG_FLOAT, float, float)
DECLARE_C_FLAG_IMPL(C_FLAG_DOUBLE, double, double)

void c_flags_set_application_name(const char *appname)
{
    c_flags_appname_message = (char *) appname;
}

void c_flags_set_positional_args_description(const char *description)
{
    c_flags_pos_args_desc = (char *) description;
}

void c_flags_set_description(const char *description)
{
    c_flags_description_message = (char *) description;
}

static CFlag *find_c_flag_by_long_name(StringView long_name)
{
    for (size_t i = 0; i < flags_size; i++) {
        CFlag *flag = &flags[i];

        if (sv_equal(sv_from_string(flag->long_name), long_name))
            return flag;
    }

    return NULL;
}

static CFlag *find_c_flag_by_short_name(StringView short_name)
{
    for (size_t i = 0; i < flags_size; i++) {
        CFlag *flag = &flags[i];

        if (sv_equal(sv_from_string(flag->short_name), short_name))
            return flag;
    }

    return NULL;
}

void c_flags_parse(int *argc_ptr, char ***argv_ptr, bool usage_on_error)
{
    int argc = *argc_ptr;
    char **argv = *argv_ptr;

    assert(argc > 0 && "argc must be grater then 0");

    int arg = 1;
    while (arg < argc) {
        StringView token = sv_from_string(argv[arg]);
        assert(token.data != NULL && "argv cannot be NULL");

        CFlag *flag = NULL;
        bool flag_long = false;
        StringView sv_value = sv_from_string(NULL);

        // `--flag value` or `--flag=value`
        if (sv_starts_with(token, sv_from_string("--"))) {
            // `--flag value`
            if (!sv_contains(token, sv_from_string("="))) {
                StringView sv_long_name = sv_chop_left(token, strlen("--"));

                flag = find_c_flag_by_long_name(sv_long_name);
                if (flag == NULL) {
                    printf("ERROR: unknown flag --" SVFMT "\n", SVARG(sv_long_name));
                    goto error;
                }

                if (flag->type != C_FLAG_BOOL) {
                    if (arg + 1 >= argc) {
                        printf("ERROR: no value for flag --" SVFMT "\n", SVARG(sv_long_name));
                        goto error;
                    }

                    sv_value = sv_from_string(argv[++arg]);
                }
            }
            // `--flag=value`
            else {
                size_t index_of_eq = (size_t) sv_index_of(token, sv_from_string("="));
                StringView sv_long_name = sv_chop_left(sv_slice_left(token, index_of_eq),
                                                       strlen("--"));

                sv_value = sv_chop_left(token, index_of_eq + 1);

                if (sv_value.data == NULL) { // `--flag=`
                    printf("ERROR: no value for flag --" SVFMT "\n", SVARG(sv_long_name));
                    goto error;
                }

                flag = find_c_flag_by_long_name(sv_long_name);
                if (flag == NULL) {
                    printf("ERROR: unknown flag --" SVFMT "\n", SVARG(sv_long_name));
                    goto error;
                }
            }

            flag_long = true;
        }
        // `-f value`
        else if (sv_starts_with(token, sv_from_string("-"))) {
            StringView sv_short_name = sv_chop_left(token, strlen("-"));

            flag = find_c_flag_by_short_name(sv_short_name);
            if (flag == NULL) {
                printf("ERROR: unknown flag -" SVFMT "\n", SVARG(sv_short_name));
                goto error;
            }

            if (flag->type != C_FLAG_BOOL) {
                if (arg + 1 >= argc) {
                    printf("ERROR: no value for flag -" SVFMT "\n", SVARG(sv_short_name));
                    goto error;
                }

                sv_value = sv_from_string(argv[++arg]);
            }
        }
        // positional arguments
        else {
            break;
        }

        char *value = (char *) sv_value.data;

        switch (flag->type) {
        case C_FLAG_INT:
            C_FLAG_LOAD_SIGNED_VALUE(flag, flag_long, int, value, usage_on_error)
            break;
        case C_FLAG_INT_8:
            C_FLAG_LOAD_SIGNED_VALUE(flag, flag_long, int8_t, value, usage_on_error)
            break;
        case C_FLAG_INT_16:
            C_FLAG_LOAD_SIGNED_VALUE(flag, flag_long, int16_t, value, usage_on_error)
            break;
        case C_FLAG_INT_32:
            C_FLAG_LOAD_SIGNED_VALUE(flag, flag_long, int32_t, value, usage_on_error)
            break;
        case C_FLAG_INT_64:
            C_FLAG_LOAD_SIGNED_VALUE(flag, flag_long, int64_t, value, usage_on_error)
            break;
        case C_FLAG_UNSIGNED:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, unsigned, value, usage_on_error)
            break;
        case C_FLAG_UINT_8:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, uint8_t, value, usage_on_error)
            break;
        case C_FLAG_UINT_16:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, uint16_t, value, usage_on_error)
            break;
        case C_FLAG_UINT_32:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, uint32_t, value, usage_on_error)
            break;
        case C_FLAG_UINT_64:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, uint64_t, value, usage_on_error)
            break;
        case C_FLAG_SIZE_T:
            C_FLAG_LOAD_UNSIGNED_VALUE(flag, flag_long, size_t, value, usage_on_error)
            break;
        case C_FLAG_BOOL:
            *C_FLAG_DATA_AS_PTR(flag, bool) = true;
            break;
        case C_FLAG_STRING:
            *C_FLAG_DATA_AS_PTR(flag, char *) = value;
            break;
        case C_FLAG_FLOAT:
            C_FLAG_LOAD_FLOATING_VALUE(flag, flag_long, float, value, strtof, usage_on_error);
            break;
        case C_FLAG_DOUBLE:
            C_FLAG_LOAD_FLOATING_VALUE(flag, flag_long, double, value, strtod, usage_on_error);
            break;
        default:
            assert(false && "not all flag types implements c_flags_parse()");
        }

        arg += 1;
    }

    *argc_ptr = argc - arg;
    *argv_ptr = argv + arg;

    return;

error:
    if (usage_on_error) {
        printf("\n");
        c_flags_usage();
    }

    exit(1);
}

static char *c_flag_default_to_str(const CFlag *flag)
{
    static char buff[32] = {0};

    switch (flag->type) {
    case C_FLAG_INT:
        snprintf(buff, sizeof(buff), "%d", *C_FLAG_DEFAULT_DATA_AS_PTR(flag, int));
        return buff;
    case C_FLAG_INT_8:
        snprintf(buff, sizeof(buff), "%" PRId8, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, int8_t));
        return buff;
    case C_FLAG_INT_16:
        snprintf(buff, sizeof(buff), "%" PRId16, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, int16_t));
        return buff;
    case C_FLAG_INT_32:
        snprintf(buff, sizeof(buff), "%" PRId32, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, int32_t));
        return buff;
    case C_FLAG_INT_64:
        snprintf(buff, sizeof(buff), "%" PRId64, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, int64_t));
        return buff;
    case C_FLAG_UNSIGNED:
        snprintf(buff, sizeof(buff), "%u", *C_FLAG_DEFAULT_DATA_AS_PTR(flag, unsigned));
        return buff;
    case C_FLAG_UINT_8:
        snprintf(buff, sizeof(buff), "%" PRIu8, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, uint8_t));
        return buff;
    case C_FLAG_UINT_16:
        snprintf(buff, sizeof(buff), "%" PRIu16, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, uint16_t));
        return buff;
    case C_FLAG_UINT_32:
        snprintf(buff, sizeof(buff), "%" PRIu32, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, uint32_t));
        return buff;
    case C_FLAG_UINT_64:
        snprintf(buff, sizeof(buff), "%" PRIu64, *C_FLAG_DEFAULT_DATA_AS_PTR(flag, uint64_t));
        return buff;
    case C_FLAG_SIZE_T:
        snprintf(buff, sizeof(buff), "%zu", *C_FLAG_DEFAULT_DATA_AS_PTR(flag, size_t));
        return buff;
    case C_FLAG_BOOL:
        return *C_FLAG_DEFAULT_DATA_AS_PTR(flag, bool) ? "true" : "false";
    case C_FLAG_STRING:
        return *C_FLAG_DEFAULT_DATA_AS_PTR(flag, char *);
    case C_FLAG_FLOAT:
        snprintf(buff, sizeof(buff), "%f", *C_FLAG_DEFAULT_DATA_AS_PTR(flag, float));
        return buff;
    case C_FLAG_DOUBLE:
        snprintf(buff, sizeof(buff), "%lf", *C_FLAG_DEFAULT_DATA_AS_PTR(flag, double));
        return buff;
    default:
        assert(false && "not all flag types implements c_flag_default_to_str()");
    }

    return "unreachable";
}

void c_flags_usage(void)
{
    if (c_flags_appname_message)
        printf("USAGE:\n   %s%s%s\n\n",
               c_flags_appname_message,
               (flags_size > 0) ? " [OPTIONS] " : " ",
               c_flags_pos_args_desc ? c_flags_pos_args_desc : "");

    if (c_flags_description_message)
        printf("DESCRIPTION:\n   %s\n\n", c_flags_description_message);

    if (flags_size > 0)
        printf("OPTIONS:");

    for (size_t i = 0; i < flags_size; i++) {
        const CFlag *flag = &flags[i];

        printf("\n   --%s", flag->long_name);
        if (flag->short_name != NULL)
            printf(", -%s", flag->short_name);
        printf("\n");

        if (flag->desc != NULL)
            printf("       Description: %s\n", flag->desc);

        char *default_val = c_flag_default_to_str(flag);
        if (default_val != NULL)
            printf("       Default: %s\n", c_flag_default_to_str(flag));
    }
}
