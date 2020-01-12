project('gimp-stx-plugin', 'cpp')
add_project_arguments('-Wno-write-strings', language : 'cpp')

src = [
  'src/main.cpp',
  'src/bytes.cpp',
  'src/stxread.cpp',
  'src/stxwrite.cpp'
]

headers = include_directories('src/headers')

gtk = dependency('gimpui-2.0')
gimp = dependency('gimp-2.0')
glib = dependency('glib-2.0')
executable('file-stx', src,
  include_directories : headers,
  dependencies : [gimp, gtk, glib])

test_bytes = ['tests/bytes/little8.cpp', 'src/bytes.cpp']
bytes_little8 = executable('test-bytes-little8',
  test_bytes,
  include_directories: headers)
test('Reading Little Endian', bytes_little8)

