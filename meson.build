project( 'fmorurl', 'c', version : '1.0.0' )


deps = []
includes = []
src = []

src += 'src/urlparser.c'

includes += 'src'


##############
# libfmorurl
libfmorurl = static_library( 'fmorurl',
                  src,
                  include_directories : includes,
                  dependencies : deps,
                  pic : true,
                  install: false,
                )
libfmorurl_dep = declare_dependency( include_directories : includes, link_with : libfmorurl, dependencies: deps )



##############
# test
if get_option( 'fmorurl_build_tests' )
    tests  = executable(  'test_urlparser', 'tests/test_urlparser.c', dependencies: libfmorurl_dep, install: false )
    example = executable( 'example', 'example/example.c', dependencies: libfmorurl_dep, install: false )
endif
