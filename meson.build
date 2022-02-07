project('c-flags', 'c', version: '1.0.0',
        default_options: ['c_std=c99', 'warning_level=3'])

src = files('c-flags.c', 'string-view.c')

bin = executable('c-flags', 'main.c', src)

# Tests setup

if add_languages('cpp') == false
    error('could not add c++ compiler to project')
endif

tests_deps = dependency('gtest')

test_default = executable('c-flags-test-default', src,
                          'tests/main.cpp', 'tests/c-flags-test-default.cpp',
                          dependencies: tests_deps)

test_short = executable('c-flags-test-short', src,
                        'tests/main.cpp', 'tests/c-flags-test-short.cpp',
                        dependencies: tests_deps)

test_long = executable('c-flags-test-long', src,
                       'tests/main.cpp', 'tests/c-flags-test-long.cpp',
                       dependencies: tests_deps)

test_long_eq = executable('c-flags-test-long-eq', src,
                          'tests/main.cpp', 'tests/c-flags-test-long-eq.cpp',
                          dependencies: tests_deps)

test_string_view = executable('string-view-tests', src,
                              'tests/main.cpp', 'tests/string-view-tests.cpp',
                              dependencies: tests_deps)

test('c-flags test default value', test_default)
test('c-flags test short name', test_short)
test('c-flags test long name', test_long)
test('c-flags test long name with equal', test_long_eq)
test('string-view tests', test_string_view)
