/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**              FF_Crazy create
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include "imageloader.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image* constructor(int column, int row) {
  Image* image = (Image*)malloc(sizeof(Image));
  image->rows = row;
  image->cols = column;
  image->image = (Color**)malloc(sizeof(Color*) * row);
  for (int i = 0; i < row; i++) {
    image->image[i] = (Color*)malloc(sizeof(Color) * column);
  }
  return image;
}

//Opens a .ppm P3 image file, and constructs an Image object.
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image* readData(char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }
  int row, column;
  char mode[3];
  int max_val;
  fscanf(file, "%s\n%d %d\n%d", mode, &column, &row, &max_val);
  Image* image = constructor(column, row);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      fscanf(file, "%hhu %hhu %hhu", &image->image[i][j].R,
             &image->image[i][j].G, &image->image[i][j].B);
    }
  }
  fclose(file);
  return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image* image) {
  printf("P3\n");
  printf("%d %d\n", image->cols, image->rows);
  printf("255\n");
  for (int i = 0; i < image->rows; i++) {
    for (int j = 0; j < image->rows; j++) {
      printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G,
             image->image[i][j].B);
      if (j < image->cols - 1) {
        printf("   ");
      }
    }
    printf("\n");
  }
}

//Frees an image
void freeImage(Image* image) {
  if (image != NULL) {
    for (int i = 0; i < image->rows; i++) {
        free(image->image[i]);
    }
    free(image->image);
    free(image);
  }
}