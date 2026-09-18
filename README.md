# USG
This project is a simplified version of the u8g2 (a veeery simple one). You can use this project to print some of the most used shapes on a matrix.

## Single Header
For the project to be easy to use and to mod. We just create a single header file with all the functions and definitions you need.
To add it to your main you need to call the `usg.h` and add the macro `#define USG_IMPLEMENTATION`, on the other files you need to just call the usg.h.

The project **lacks string handlers**. As I said is a work in progress.

## Examples
1. Demo: In this demo you can find the general shapes print on the terminal. You may want to zoom out on the terminal to see all the "pixels".
2. Sdl: This implementation shows the use of sdl2 with the usg library. Printing a circle that can increase or decrease its radius with the arrow keys.
