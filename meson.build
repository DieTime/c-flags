# SPDX-FileCopyrightText: Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

project('c-flags', ['c', 'cpp'],
         default_options: ['c_std=c99', 'cpp_std=c++14', 'warning_level=3'],
         version: '1.5.6',
         license: 'MIT')

subdir('lib')

if get_option('tests')
    subdir('tests')
endif

if get_option('examples')
    subdir('examples')
endif
