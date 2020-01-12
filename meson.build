project('gimp-stx-plugin', 'cpp')
add_project_arguments('-Wno-write-strings', language : 'cpp')

src = ['src/main.cpp', 'src/bytes.cpp', 'src/stxread.cpp']

headers = include_directories('src/headers')

gimp = dependency('gimp-2.0')
executable('file-stx', src,
  include_directories : headers,
  dependencies : [gimp])

test_bytes = ['tests/bytes/little8.cpp', 'src/bytes.cpp']
bytes_little8 = executable('test-bytes-little8',
  test_bytes,
  include_directories: headers)
test('Reading Little Endian', bytes_little8)

