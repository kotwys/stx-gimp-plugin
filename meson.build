project('stx-gimp-plugin', 'cpp')
add_global_arguments('-std=c++17', language : 'cpp')
# Support enum values combining
add_project_arguments('-fpermissive', language : 'cpp')

# Sources

sources = [
  'src/main.cpp',
  'src/gimp_interop.cpp',
  'src/dialog.cpp'
]
include = include_directories('include')

# Dependencies

gimp_dep = dependency('gimp-2.0')
gimpui_dep = dependency('gimpui-2.0')
giomm_dep = dependency('giomm-2.4')

ui_proj = subproject('ui')
ui_dep = ui_proj.get_variable('ui_dep')

stx_proj = subproject('stx')
stx_dep = stx_proj.get_variable('stx_dep')

neither_proj = subproject('neither')
neither_dep = neither_proj.get_variable('neither_dep')

# Outputs

executable('file-stx', sources,
  include_directories : include,
  dependencies : [
    gimp_dep,
    gimpui_dep,
    giomm_dep,
    ui_dep,
    stx_dep,
    neither_dep
  ])
