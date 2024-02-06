## Pixel Text Gimp Plugin
Make retro gaming text in Gimp using this plugin 

![image preview](/previews/preview0.png)

![image preview](/previews/preview1.png)

## Location to put binaries (they don't go in the normal plugins folder)

Windows

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins
 
 Linux 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak includes Chromebook)

 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins


Then restart Gimp and go to GEGL operation and look for "pixel text"


## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).

BEAVER RECOMMENDS YOU USE A MODERN VERSION OF GEGL. NO GUARANTEE DATED VERSIONS OF GIMP WILL WORK WITH THIS PLUGIN 

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## another preview of this based plugin

![image preview](/previews/preview2.png)



