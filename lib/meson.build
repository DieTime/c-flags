src = files('c-flags.c', 'string-view.c')

lib = shared_library('c-flags', src, install: true)

install_headers('c-flags.h')

install_data('c-flags.pc',
             install_dir: get_option('prefix') / get_option('datadir') / 'pkgconfig')

cflags_inc = include_directories('.')

cflags_lib = declare_dependency(link_with: lib,
                                include_directories: cflags_inc)
