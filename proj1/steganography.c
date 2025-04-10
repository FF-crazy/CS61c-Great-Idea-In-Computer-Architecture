/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				FF_Crazy
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color* evaluateOnePixel(Image* image, int row, int col) {
  uint8_t blue_val = image->image[row][col].B;
  if (blue_val & 1) {
    Color* temp = (Color*)malloc(sizeof(Color));
    temp->R = 255;
    temp->G = 255;
    temp->B = 255;
    return temp;
  } else {
    Color* temp = (Color*)malloc(sizeof(Color));
    temp->R = 0;
    temp->G = 0;
    temp->B = 0;
    return temp;
  }
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image* steganography(Image* image) {
  Image* res = (Image*)malloc(sizeof(Image));
  res->cols = image->cols;
  res->rows = image->rows;
  res->image = (Color**)malloc(res->rows * sizeof(Color*));
  for (int i = 0; i < res->rows; i++) {
    res->image[i] = (Color*)malloc(image->cols * sizeof(Color));
  }
  for (int i = 0; i < res->rows; i++) {
    for (int j = 0; j < res->cols; j++) {
      Color* color = evaluateOnePixel(image, i, j);
      res->image[i][j].R = color->R;
      res->image[i][j].G = color->G;
      res->image[i][j].B = color->B;
      free(color);
    }
  }
  return res;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char** argv) {
  if (argc != 2) {
    return -1;
  }
  Image* image = readData(argv[1]);
  Image* res = steganography(image);
  writeData(res);
  freeImage(image);
  freeImage(res);
  return 0;
}
