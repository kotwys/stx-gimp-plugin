load('//:buckaroo_macros.bzl', 'buckaroo_deps')
load('//:subdir_glob.bzl', 'subdir_glob')

cxx_binary(
  name = 'file-stx',
  compiler_flags = [
    '-fpermissive'
  ],
  headers = subdir_glob([
    ('src/headers', '**/*.h')
  ]),
  header_namespace = '',
  srcs = [
    'src/main.cpp',
    'src/dialog.cpp',
    'src/gimp_interop.cpp'
  ],
  deps = [
    '//src/stx:stx',
    '//src/ui:ui',
    '//external/gimp-2.0:gimp-2.0',
    '//external/gimpui-2.0:gimpui-2.0',
    '//external/gtkmm-2.0:gtkmm-2.0'
  ] + buckaroo_deps()
)
