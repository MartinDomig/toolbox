project('my_toolbox', 'c')

deps = [
    dependency('glib-2.0'),
    dependency('gio-2.0')
]

sources = files([
    'src/main.c',
    'src/ntp.c',
])

executable('demo',
    sources,
    dependencies: deps
)