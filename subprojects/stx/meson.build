project('stx', 'cpp')

# Sources

sources = [
  'src/stx/read.cpp',
  'src/stx/write.cpp',
  'src/utils/bytes.cpp',
]
include = include_directories('include')

# Dependencies

giomm_dep = dependency('giomm-2.4')

neither_proj = subproject('neither')
neither_dep = neither_proj.get_variable('neither_dep')

# Outputs

stx = static_library('stx', sources,
  include_directories : include,
  dependencies : [giomm_dep, neither_dep])

stx_dep = declare_dependency(link_with : stx,
  include_directories : include,
  dependencies : [giomm_dep, neither_dep])

# Tests

test_bytes = executable('test-bytes',
  ['src/utils/bytes.cpp', 'src/utils/bytes.test.cpp'],
  include_directories : include)
test('Reading bytes', test_bytes)
