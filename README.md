# STX plugin for GIMP

This plugin allows to load textures in `.stx` format. It's used, for example,
in Winx Club™ PC game (2006).

**This plugin is written for pre-2.10 API**

![Screenshot](assets/screenshot.png)

## BUILDING

The plugin is built with [Buck](https://buck.build).
[Buckaroo](https://buckaroo.pm) is used as a package manager. You also need to
install system's `libgimp`. The other dependencies should be installed
along with it. Your pkg-config then must include:

+ `gimp-2.0`
+ `gimpui-2.0`
