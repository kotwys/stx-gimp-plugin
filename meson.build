project('file-stx', 'c',
        version : '1.0.0',
        license : 'GPL-3.0-or-later',
        meson_version : '>=0.60.0')

plugin_name = 'file-stx'

splendente_prefix = get_option('splendente_prefix')
if splendente_prefix == ''
  error('-Dsplendente_prefix is not specified but is required.')
endif

# Splendente

cc       = meson.get_compiler('c')
stx_dep  = declare_dependency(
  dependencies : [cc.find_library('splendente-stx',
                                  dirs : splendente_prefix / 'lib',
                                  static : true)],
  include_directories : [splendente_prefix / 'include'],
)

# System dependencies

gimp_dep = dependency('gimp-3.0')
gimpui_dep = dependency('gimpui-3.0')
imagequant_dep = dependency('imagequant', static : true)

sources = files(
  'file-stx/babl-helper.c',
  'file-stx/stx-export-dialog.c',
  'file-stx/stx-export.c',
  'file-stx/stx-load.c',
  'file-stx/stx-settings.c',
  'file-stx/stx.c',
  'file-stx/swizzle.c',
)

executable(plugin_name, sources,
           dependencies : [gimp_dep, gimpui_dep, imagequant_dep, stx_dep],
           win_subsystem : 'windows',
           install : true,
           install_dir : plugin_name)

subdir('po')
