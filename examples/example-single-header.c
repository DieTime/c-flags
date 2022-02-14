#include <inttypes.h>
#include <stdio.h>

#include "../single-header/c-flags.h"

int main(int argc, char *argv[])
{
    c_flags_add_info("program", "<file-path>");

    bool *verbose = c_flag_bool("verbose", "v", "verbose mode", false);
    uint64_t *batch = c_flag_uint64("batch-size", "bs", "declare batch size", 32);
    size_t *offset = c_flag_size_t("offset", "off", "declare file offset", 0);
    bool *help = c_flag_bool("help", "h", "show usage", false);

    c_flags_parse(&argc, &argv, false);

    if (*help) {
        c_flags_usage();
        return 0;
    }

    if (argc == 0) {
        printf("ERROR: required file path not specified\n\n");
        c_flags_usage();
        return 1;
    }

    char *file_path = argv[0];

    printf("Parsed command line arguments and flags:\n");
    printf("   verbose:    %s\n", *verbose ? "true" : "false");
    printf("   batch-size: %" PRIu64 "\n", *batch);
    printf("   offset:     %zu\n", *offset);
    printf("   file-path:  %s\n", file_path);

    return 0;
}
