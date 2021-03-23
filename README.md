# STX plugin for GIMP

This plugin allows to load textures in `.stx` format. It's used, for example,
in Winx Club™ PC game (2006).

![Screenshot](assets/screenshot.png)

## INSTALLATION

In order to install the plugin you need download the prepared redistributables
from [Releases page](https://github.com/kotwys/stx-gimp-plugin/releases) and
put it in plugins folder of your GIMP installation. Usually it is

- On Windows: `C:\Users\{your username}\Appdata\Roaming\GIMP\{gimp version}\plug-ins`
- On macOS: `/Users/{your username}/Library/GIMP/{gimp version}/plug-ins`
- On Linux: `/home/{your username}/.local/share/GIMP/{gimp version}/plug-ins`

If not, you can look for appropriate folders in GIMP settings.

The redistributables are prepared for Windows. If you're using macOS or Linux,
you'll need to build the plugin yourself. Requirements are covered in [building
section](#building).


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
