# SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

sources = ['c-flags.c', 'string-view.c']
headers = ['c-flags.h']

compile_args_common = []
compile_args_target = []

if get_option('default_library') == 'shared'
    compile_args_common += ['-DC_FLAGS_USE_SHARED_LIBRARY']
    compile_args_target += ['-DC_FLAGS_BUILD_SHARED_LIBRARY'] + compile_args_common
endif

libcflags = library(
    'c-flags',
    sources,
    version: meson.project_version(),
    c_args: compile_args_target,
    gnu_symbol_visibility: 'hidden',
    install: true,
)

libcflags_dep = declare_dependency(
    compile_args: compile_args_common,
    link_with: libcflags,
    include_directories: include_directories('.'),
)

pkg = import('pkgconfig')
pkg.generate(
    libcflags,
    description: 'Library for parsing command line arguments easily',
)

install_headers(headers)
