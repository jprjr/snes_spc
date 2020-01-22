# snes_spc

This is a fork of [snes_spc](http://www.slack.net/~ant/libs/audio.html#snes_spc).

More details in the original `readme.txt` file. I've removed the fast DSP,
and added a script to create an amalgamated build (so you can generate a single
.cpp file to bring into your projects).

Also fun fact, if you compile with `-fno-exceptions` and `-fno-rtti` you can
avoid libstdc++ entirely - you can have an object file that can be brought
into a C project without any dependencies, save for some stdlib functions.

This does not parse id666 tags found in .spc files, I have a separate c library
to handle that.

# LICENSE

LGPL 2.1 (see `license.txt`).
