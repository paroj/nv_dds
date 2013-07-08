nv_dds
======

DDS image loader for OpenGL/ OpenGL ES2

this project tracks updates/ bugifxes for the DDS loader from the nvidia SDK 
to make it usable with OpenGL3+

detailled changes:
* refactor code for faster execution/ smaller footprint
* fix for running on 64bit
* fix compilation with OpenGL ES2
* correctly handle RGBA, BGRA, RGB, BGR formats (some require ES2 extensions)
* add interface to load files from memory
