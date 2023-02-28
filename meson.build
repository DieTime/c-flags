project('c-flags', 'c', version: '1.3.0', license: 'MIT',
        default_options: ['c_std=c99', 'warning_level=3'])

subdir('lib')

if get_option('tests')
    subdir('tests')
endif

if get_option('examples')
    subdir('examples')
endif
