##Description

The project implements the idea of walking from point A to point B in some 
space via the shortest path.

It's up to you to decide which space to go through. The only constraint is that it must inherit
**Surface** class defined in *include/Surface.h*.

There's only one path finding algorithm supported at the moment - A* - and it's very efficient. 

##Usage

1. Run `make` in the root directory. Hopefully, this will generate *bin*, *build*, *lib* directories. 
2. Go to *test* directory and run `./test` to make sure everything work fine.
3. Use dynamic libraries from *lib* directory: it contains several predefined 
surfaces like Plane, Cylinder and Torus.

***
To test how predefined surfaces work, go to *bin* directory and run `./main`.
You'll get a clear help message with instructions like these:
```
 -h --help      Print help
 -i --in        Input file with space description
 --o --out      File to write route to
 -t --topology  Space topology [plane | cylinder | torus | words]
 -l -limit      Maximum route length
```
To see how surfaces should be defined, go to *test* directory and look at the examples.
