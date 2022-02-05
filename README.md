# FZ_CHESS
 _A GUI based Chess game written in C++(17)_

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

## Features

- Cross Platform Compatible
- Uses SDL2 and SDL2_Image as the rendering Library
- Portable to macOS, Linux and Android

## Tech

FZ_CHESS uses a number of open source projects to work properly:

- [SDL2] - Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to input and graphics hardware via OpenGL and Direct3D.
- [SDL Image] - SDL_image is an image file loading library. 

And of course FZ_CHESS itself is open source with a public repository [FZ_CHESS][fzchess] on GitHub.

## Pre-Requisites for Building

Building the game executable from source requires a C++ compiler along with SDL2 and SDL2_Image Development Libraries and a "Make" utility.

Firstly, make sure a C++ compiler is installed on your computer (GNU GCC 10.2.0+ preferred) which supports the C++17 standard.

To verify whether a compiler is installed, run a terminal and type :-

```sh
g++ --version
```
If you encounter an error message stating "command not found", you might need to re-install the compiler. Else you should be good.

Next, verify that the "make" utility is installed and working correctly. 


For verifying, run a terminal and type ...

```sh
make
```
If u get an output stating ...
```sh
make: *** No targets specified and no makefile found.  Stop.
```
then the make utility is working correctly.

The SDL2 and SDL_Image Development Libraries are already included with the Source Code. Installing these Libraries manually is not necessary. 

## Building form source

Download the Source Code from the [fzchess][main] branch. Open up a terminal and navigate to the main folder.
Type :

```sh
make
```

and it should create the Game Executable called "main.exe" in the main folder.
Open "main.exe" to run the game....Enjoy!!!


## Development

Want to contribute? Great!

If you have any Suggestions, or encounter any bugs, or just want to give feedback please do open a PR and I will look into it.
Feel free to fork/clone this repo.
Give it a star if u find this project useful and fun.

Lastly, look out for more projects coming shortly.

`FZ_CHESS -version 1.0`

## License

FZ_CHESS itself does not enforce any License. Use its source as you see fit.
However, SDL 2.0 and SDL_Image are provided under Zlib license and are properties of their respective owners.
Please visit [SDL_Licensing][sdllicense] for more info.


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [fzchess]: <https://github.com/fraz-uddin07/FZ_CHESS/tree/master>
   [sdllicense]: <https://www.libsdl.org/license.php>
