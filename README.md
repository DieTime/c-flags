<p align="center">
    <img src="assets/icon.png" width="400px" style="margin-bottom: 5px;" />
</p>

<p align="center">
    <img src="https://github.com/DieTime/c-flags/actions/workflows/health-check.yml/badge.svg" alt="health check status" />
    <img src="https://img.shields.io/github/license/DieTime/c-flags?color=%231cc727" alt="License type" />
    <img src="https://img.shields.io/badge/supports-single%20header-green?color=%231cc727" alt="License type" />
</p>

Library for parsing command line arguments easily. Like golang `flag` module.

>❗ Currently working and tested on `Linux`

>👍 Supports [single header](single-header/c-flags.h) version

# 🔨 Building and installing

```bash
$ meson builddir && cd builddir
$ meson compile
$ sudo meson install
```

# 🔌 Linking

The library supports pkg-config, which makes linking easier and more convenient.

```cmake
cmake_minimum_required(VERSION 3.14)
project(program)

find_package(PkgConfig)
pkg_search_module(C_FLAGS REQUIRED c-flags)

add_executable(program examples/example.c)

target_compile_options(program PUBLIC ${C_FLAGS_CFLAGS})
target_link_libraries(program PUBLIC ${C_FLAGS_LIBRARIES})
```

# 👨‍💻 Usage

You can see examples sources [here](examples).

```c
#include <c-flags.h>
#include <inttypes.h>
#include <stdio.h>

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

```

# 💻 Usage output example

```
USAGE:
   program [OPTIONS] <file-path>

OPTIONS:
  --verbose, -v
      Description: verbose mode
      Default: false

  --batch-size, -bs
      Description: declare batch size
      Default: 32

  --offset, -off
      Description: declare file offset
      Default: 0

  --help, -h
      Description: show usage
      Default: false
```

## ☕ Support

You can support me so that there will be more good open source projects in the future
<p align="center" style="padding: 10px 0 20px 0">
  <a href="https://www.buymeacoffee.com/glazzkoff" target="_blank">
    <img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="50" width="220">
  </a>
</p>
