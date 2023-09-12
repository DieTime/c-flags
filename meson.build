project('c-flags', ['c', 'cpp'], version: '1.5.5', license: 'MIT',
        default_options: ['c_std=c99', 'cpp_std=c++14', 'warning_level=3'])

subdir('lib')

if get_option('tests')
    subdir('tests')
endif

if get_option('examples')
    subdir('examples')
endif
