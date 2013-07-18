nv_dds
======

## Overview
this project tracks updates/ bugfixes for the DDS loader from the nvidia SDK and ports it to OpenGL2+ and OpenGL ES2

### changes
* refactor code for faster execution/ smaller footprint
* gcc 64bit compability
* compability with OpenGL ES2
* correctly handle RGBA, BGRA, RGB, BGR formats (some require ES2 extensions)
* add interface to load files from memory
