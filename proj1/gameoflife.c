/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**							FF_Crazy
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "imageloader.h"

void print_error_massage() {
  printf("usage: ./gameOfLife filename rule\n");
  printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
  printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
}

void bit_check(int arr[], Color color, char c) {
  uint8_t value;
  switch (c) {
    case 'R':
      value = color.R;
      for (int i = 0; i < 8; i++) {
        int temp = value & 1;
        arr[i] += temp;
        value >>= 1;
      }
      break;
    case 'G':
      value = color.G;
      for (int i = 0; i < 8; i++) {
        int temp = value & 1;
        arr[i + 8] += temp;
        value >>= 1;
      }
      break;
    case 'B':
      value = color.B;
      for (int i = 0; i < 8; i++) {
        int temp = value & 1;
        arr[i + 16] += temp;
        value >>= 1;
      }
      break;
  }
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color* evaluateOneCell(Image* image, int row, int col, uint32_t rule) {
  Color original_color = image->image[row][col];
  int around_count[24] = {0};
  int self_color[24] = {0};
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (row + i < 0 || row + i > image->rows - 1 || col + j < 0 ||
          col + j > image->cols - 1 || (i == 0 && j == 0)) {
        continue;
      } else {
        bit_check(around_count, image->image[row + i][col + j], 'R');
        bit_check(around_count, image->image[row + i][col + j], 'G');
        bit_check(around_count, image->image[row + i][col + j], 'B');
      }
    }
  }
  bit_check(self_color, original_color, 'R');
  bit_check(self_color, original_color, 'G');
  bit_check(self_color, original_color, 'B');
  Color* new_color = (Color*)malloc(sizeof(Color));
  new_color->R = 0, new_color->G = 0, new_color->B = 0;
  int temp[24] = {0};
  for (int i = 0; i < 24; i++) {
    if (self_color[i]) {
      uint8_t this_color = (rule >> around_count[i] + 9) & 1;
      temp[i] = this_color;
    } else {
      uint8_t this_color = (rule >> around_count[i]) & 1;
      temp[i] = this_color;
    }
  }
  for (int i = 7; i >= 0; i++) {
    new_color->R += temp[i + 16];
    new_color->R <<= 1;
    new_color->G += temp[i + 8];
    new_color->G <<= 1;
    new_color->B += temp[i];
    new_color->B <<= 1;
  }
  return new_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image* life(Image* image, uint32_t rule) {
  Image* res = (Image*)malloc(sizeof(Image));
  res->cols = image->cols;
  res->rows = image->rows;
  res->image = (Color**)malloc(res->rows * sizeof(Color*));
  for (int i = 0; i < res->rows; i++) {
    res->image[i] = (Color*)malloc(image->cols * sizeof(Color));
  }
  for (int i = 0; i < res->rows; i++) {
    for (int j = 0; j < res->cols; j++) {
      Color* color = evaluateOneCell(image, i, j, rule);
      res->image[i][j].R = color->R;
      res->image[i][j].G = color->G;
      res->image[i][j].B = color->B;
      free(color);
    }
  }
  return res;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char** argv) {
  if (argc != 3) {
    print_error_massage();
    return -1;
  }
  Image* origin_image = readData(argv[1]);
  if (!origin_image) {
    print_error_massage();
    return -1;
  }
  char* endptr;
  uint32_t rule = strtol(argv[2], &endptr, 0);
  if (!*endptr) {
    print_error_massage();
    return -1;
  }
  if (rule > 0x3FFFF) {
    print_error_massage();
    return -1;
  }
  Image* processed_image = life(origin_image, rule);
  writeData(processed_image);
  freeImage(origin_image);
  freeImage(processed_image);
  return 0;
}
