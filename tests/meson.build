# SPDX-FileCopyrightText: 2023-2025 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: MIT

add_languages('cpp', native: false, required: true)

libgtest_dep = dependency('gtest', fallback: 'gtest')
dependencies = [libgtest_dep, libcflags_dep]

test_default = executable(
    'c-flags-test-default',
    'main.cpp',
    'c-flags-test-default.cpp',
    dependencies: dependencies,
)

test_short = executable(
    'c-flags-test-short',
    'main.cpp',
    'c-flags-test-short.cpp',
    dependencies: dependencies,
)

test_long = executable(
    'c-flags-test-long',
    'main.cpp',
    'c-flags-test-long.cpp',
    dependencies: dependencies,
)

test_long_eq = executable(
    'c-flags-test-long-eq',
    'main.cpp',
    'c-flags-test-long-eq.cpp',
    dependencies: dependencies,
)

test_no_value = executable(
    'c-flag-test-no-value',
    'main.cpp',
    'c-flags-test-no-value.cpp',
    dependencies: dependencies,
)

test_string_view = executable(
    'string-view-tests',
    'main.cpp',
    'string-view-tests.cpp',
    '../lib/string-view.c',
    include_directories: ['../lib'],
    dependencies: [libgtest_dep],
)

test('c-flags test default value', test_default)
test('c-flags test short name', test_short)
test('c-flags test long name', test_long)
test('c-flags test long name with equal', test_long_eq)
test('c-flags test no value', test_no_value)
test('string-view tests', test_string_view)
