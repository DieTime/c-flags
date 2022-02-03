#ifndef C_FLAGS_H_
#define C_FLAGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef enum {
    C_FLAG_INT,
    C_FLAG_INT_8,
    C_FLAG_INT_16,
    C_FLAG_INT_32,
    C_FLAG_INT_64,
    C_FLAG_SSIZE_T,
    C_FLAG_UNSIGNED,
    C_FLAG_UINT_8,
    C_FLAG_UINT_16,
    C_FLAG_UINT_32,
    C_FLAG_UINT_64,
    C_FLAG_SIZE_T,
    C_FLAG_BOOL,
    C_FLAG_STRING,
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

// clang-format off
/**
 * Declare `c_flag_*` function definition for any type.
 *
 * @param ptr_type Flag type (size_t, int, bool, ...)
 * @param postfix Function name postfix
 */
#define DECLARE_C_FLAG_DEF(ptr_type, postfix)              \
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
DECLARE_C_FLAG_DEF(ssize_t, ssize_t)
DECLARE_C_FLAG_DEF(unsigned, unsigned)
DECLARE_C_FLAG_DEF(uint8_t, uint8)
DECLARE_C_FLAG_DEF(uint16_t, uint16)
DECLARE_C_FLAG_DEF(uint32_t, uint32)
DECLARE_C_FLAG_DEF(uint64_t, uint64)
DECLARE_C_FLAG_DEF(size_t, size_t)
DECLARE_C_FLAG_DEF(bool, bool)
DECLARE_C_FLAG_DEF(char *, string)

/**
 * Add application name and positional arguments description
 * for customize program usage: `app_name` [OPTIONS] `positional_args_desc`.
 *
 * @param app_name Application name
 * @param positional_args_desc Positional arguments description (may be NULL)
 */
void c_flags_add_info(const char *app_name, const char *positional_args_desc);

/**
 * Parse command line arguments into declared arguments.
 * This function mutate `argc` and `argv` for comfortably positional arguments parsing.
 *
 * @param argc_ptr Pointer to program argc
 * @param argv_ptr Pointer to program argv
 * @param usage_on_error Show usage on parsing error
 */
void c_flags_parse(int *argc_ptr, char ***argv_ptr, bool usage_on_error);

/**
 * Show usage based on your declared flags.
 */
void c_flags_usage(void);

#ifdef __cplusplus
}
#endif

#endif // C_FLAGS_H_
