compiler = meson.get_compiler('c')

sources = [ 'c-flags.c', 'string-view.c' ]
headers = [ 'c-flags.h' ]

build_args = []

if get_option('default_library') == 'shared'
    use_msvc_defines = compiler.get_id() == 'msvc'
    build_args += use_msvc_defines ? '/DC_FLAGS_BUILD_SHARED' : '-DC_FLAGS_BUILD_SHARED'
endif

libcflags = library('c-flags', sources,
                     version: meson.project_version(),
                     c_args: build_args,
                     gnu_symbol_visibility: 'hidden',
                     install: true)

libcflags_dep = declare_dependency(link_with: libcflags,
                                   include_directories: ['.'])

pkg = import('pkgconfig')
pkg.generate(libcflags, description: 'Library for parsing command line arguments easily')

install_headers(headers)
