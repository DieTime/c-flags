if add_languages('cpp') == false
    error('could not add c++ compiler to project')
endif

deps = [
    dependency('gtest'),
    cflags_lib
]

test_default = executable('c-flags-test-default',
                          'main.cpp', 'c-flags-test-default.cpp',
                          dependencies: deps)

test_short = executable('c-flags-test-short',
                        'main.cpp', 'c-flags-test-short.cpp',
                        dependencies: deps)

test_long = executable('c-flags-test-long',
                       'main.cpp', 'c-flags-test-long.cpp',
                       dependencies: deps)

test_long_eq = executable('c-flags-test-long-eq',
                          'main.cpp', 'c-flags-test-long-eq.cpp',
                          dependencies: deps)

test_no_value = executable('c-flag-test-no-value',
                           'main.cpp', 'c-flags-test-no-value.cpp',
                           dependencies: deps)

test_string_view = executable('string-view-tests',
                              'main.cpp', 'string-view-tests.cpp',
                              dependencies: deps)

test('c-flags test default value', test_default)
test('c-flags test short name', test_short)
test('c-flags test long name', test_long)
test('c-flags test long name with equal', test_long_eq)
test('c-flags test no value', test_no_value)
test('string-view tests', test_string_view)
