project('ui', 'c')

sources = [
  'src/ui/stxsavedialog.c',
  'src/ui/stxnumberfield.c',
  'src/stxconfig.c'
]
headers = include_directories('include')

gtk_dep = dependency('gtk+-2.0')

ui = static_library('ui', sources,
  include_directories : headers,
  dependencies : [gtk_dep])

ui_dep = declare_dependency(link_with : ui,
  include_directories : headers,
  dependencies : [gtk_dep])
