sources = [ 'c-flags.c', 'string-view.c' ]
headers = [ 'c-flags.h' ]

cflags = shared_library('c-flags', sources, version: meson.project_version(), install: true)
cflags_dep = declare_dependency(link_with: cflags, include_directories: ['.'])

pkg = import('pkgconfig')
pkg.generate(cflags, description: 'Library for parsing command line arguments easily')

install_headers(headers)
