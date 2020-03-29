# STX plugin for GIMP

This plugin allows to load textures in `.stx` format. It's used, for example,
in Winx Club™ PC game (2006).

![Screenshot](assets/screenshot.png)

## BUILDING

> To build this plugin you need C++17-compatible compiler.

The plugin is built with [Meson®](https://mesonbuild.com). You'll also need to
install GIMP's development files (search for `libgimp`, `gimp-devel` etc.),
[glibmm libraries](https://gtkmm.org/en). Your pkg-config should then include:

+ `gimp-2.0`
+ `gimpui-2.0`
+ `gtk+-2.0`
+ `giomm-2.4`

Project depends on [Neither library](https://github.com/LoopPerfect/neither).
Follow instructions in `subprojects/neither` directory.
