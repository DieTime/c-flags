/*
 * SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#ifndef C_FLAGS_H
#define C_FLAGS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(C_FLAGS_USE_SHARED_LIBRARY)
    #if defined(C_FLAGS_BUILD_SHARED_LIBRARY)
        #if defined(_MSC_VER)
            #define C_FLAGS_EXPORT extern __declspec(dllexport)
        #elif defined(__GNUC__)
            #define C_FLAGS_EXPORT __attribute__((visibility("default")))
        #else
            #define C_FLAGS_EXPORT
        #endif
    #else
        #if defined(_MSC_VER)
            #define C_FLAGS_EXPORT extern __declspec(dllimport)
        #else
            #define C_FLAGS_EXPORT
        #endif
    #endif
#else
    #define C_FLAGS_EXPORT
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// clang-format off
/**
 * Declare `c_flag_*` function definition for any type.
 *
 * @param ptr_type Flag type (size_t, int, bool, ...)
 * @param postfix Function name postfix
 */
#define DECLARE_C_FLAG_DEF(ptr_type, postfix)              \
    C_FLAGS_EXPORT                                         \
    ptr_type *c_flag_##postfix(const char *long_name,      \
                               const char *short_name,     \
                               const char *desc,           \
                               const ptr_type default_val);
// clang-format on

DECLARE_C_FLAG_DEF(int, int)
DECLARE_C_FLAG_DEF(int8_t, int8)
DECLARE_C_FLAG_DEF(int16_t, int16)
DECLARE_C_FLAG_DEF(int32_t, int32)
DECLARE_C_FLAG_DEF(int64_t, int64)
DECLARE_C_FLAG_DEF(unsigned, unsigned)
DECLARE_C_FLAG_DEF(uint8_t, uint8)
DECLARE_C_FLAG_DEF(uint16_t, uint16)
DECLARE_C_FLAG_DEF(uint32_t, uint32)
DECLARE_C_FLAG_DEF(uint64_t, uint64)
DECLARE_C_FLAG_DEF(size_t, size_t)
DECLARE_C_FLAG_DEF(bool, bool)
DECLARE_C_FLAG_DEF(char *, string)
DECLARE_C_FLAG_DEF(float, float)
DECLARE_C_FLAG_DEF(double, double)

/**
 * Customize usage block of help message.
 * The final help message will contain the following block:
 *
 *  USAGE:
 *     <text defined by you> [OPTIONS] ...
 *
 * @param appname Application name of the usage block
 */
C_FLAGS_EXPORT
void c_flags_set_application_name(const char *appname);

/**
 * Customize usage block of help message.
 * The final help message will contain the following block:
 *
 * In order for the usage block to be printed in the help
 * message, you must set the application name using
 * the `c_flags_set_application_name()` function.
 *
 *  USAGE:
 *     ... [OPTIONS] <text defined by you>
 *
 * @param description Postitional arguments description of the usage block
 */
C_FLAGS_EXPORT
void c_flags_set_positional_args_description(const char *description);

/**
 * Customize description block of help message.
 * The final help message will contain the following block:
 *
 *  DESCRIPTION:
 *     <text defined by you>
 *
 * @param description Text of the usage block
 */
C_FLAGS_EXPORT
void c_flags_set_description(const char *description);

/**
 * Parse command line arguments into declared arguments.
 * This function mutate `argc` and `argv` for comfortably positional arguments parsing.
 *
 * @param argc_ptr Pointer to program argc
 * @param argv_ptr Pointer to program argv
 * @param usage_on_error Show usage on parsing error
 */
C_FLAGS_EXPORT
void c_flags_parse(int *argc_ptr, char ***argv_ptr, bool usage_on_error);

/**
 * Show usage based on your declared flags.
 */
C_FLAGS_EXPORT
void c_flags_usage(void);

#ifdef __cplusplus
}
#endif

#endif // C_FLAGS_H
