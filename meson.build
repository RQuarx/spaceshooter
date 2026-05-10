project('spaceshooter', 'cpp', 'c',
        version:           '0.0.1',
        license:           'GPL-3.0-or-later',
        default_options: [ 'cpp_std=c++23', 'c_std=c23', 'warning_level=3' ])

include = include_directories('include', 'shaders')
subdir('src')

args = [ '-Wno-missing-designated-field-initializers' ]
deps = [ dependency('sdl3'),
         dependency('sdl3_ttf', fallback: [ 'sdl3_ttf', 'sdl3_ttf_dep' ]),
         dependency('glm',      fallback: [ 'glm',      'glm_dep'      ]) ]

subdir('shaders')

shaders = static_library('shaders', shaders_src)

executable(meson.project_name(), src,
           include_directories: include,
           dependencies:        deps,
           link_with:           shaders,
           cpp_args:            args,
           install:             true)