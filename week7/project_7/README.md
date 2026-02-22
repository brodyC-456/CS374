# Brody's Grayscaler

## Building

Command line:

* `make` to build. An executable called `grayscaler` will be produced.

## Files

_[List files here, even if you only have one. Headers are optional.]_

* `grayscaler.c`: The main code to convert ppm images to grayscale
* `ppm.c`: API code for handling ppm image reading, manipulation, and writing.
* `ppm.h`: Includes macros for handling specific rgb values of pixels

## Data


There is a 2D array that contains all of the threads created during image processing, and a 2D array of structs containing the thread arguments for each thread.
These arrays are used to slice the image up between a number of threads equal to the user inputted thread number.
There's also argv[] containing user inputted arguments.


## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`
  * `Usage()`: Describes the way to use the tool in the command line
  * `run()`: Specifies the work each thread does
      * `process_rows()`: Uses the ppm api to convert every pixel in the image to gray
  * `calc_and_print_averages`: Takes the total rgb values calculated by the threads and prints their average

## Notes

* PPM images cannot be viewed in most IDEs or filesystems, so use `convert` or `magick` on unix command line to convert them into png images.
