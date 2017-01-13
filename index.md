## Overview
this project tracks updates/ bugfixes for the DDS loader from the nvidia SDK and ports it to OpenGL2+ and OpenGL ES2

sample usage

```
 CDDSImage image;
 GLuint texobj;

 image.load("compressed_or_uncompressed.dds");

 glGenTextures(1, &texobj);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, texobj);

 image.upload_texture2D();
```

### changes
* refactor code for faster execution/ smaller footprint
* gcc 64bit compability
* compability with OpenGL ES2
* correctly handle RGBA, BGRA, RGB, BGR formats (some require ES2 extensions)
* add interface to load files from memory
