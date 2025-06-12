# SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

project(
    'c-flags',
    'c',
    default_options: ['c_std=c99', 'cpp_std=c++17', 'warning_level=3'],
    version: '1.5.10',
    license: 'MIT',
    meson_version: '>=0.54.0',
)

subdir('lib')

if get_option('tests')
    subdir('tests')
endif

if get_option('examples')
    subdir('examples')
endif
