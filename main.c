/**
   \file main.c
   \brief Programme illustrant les capacites de generateur de commentaires de doxygen
   \author Vincent Boyer

   \author Email  : boyer@ai.univ-paris8.fr
   \date 09/2015
*/




#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "readwritepng.h"
#include "SixthConfig.h"

#define DEBUG if (1) printf

void process_png_file(ImagePNG *Image) {
  for(int y = 0; y < Image->height; y++) {
    png_bytep row = Image->row_pointers[y];
    for(int x = 0; x < Image->width; x++) {
      png_bytep px = &(row[x * 4]);
      // Do something awesome for each pixel here...
      //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
      px[0] = 255;
    }
  }
}

//On conserve la transparence
//Y = 0.2126 R + 0.7152 G + 0.0722 B
void convertBW(ImagePNG *Image){
  for(int y = 0; y < Image->height; y++) {
    png_bytep row = Image->row_pointers[y];
    for(int x = 0; x < Image->width; x++) {
      png_bytep px = &(row[x * 4]);
      px[0] = px[1] = px[2] = 0.2126 *px[0] + 0.7152*px[1] + 0.0722*px[2];
    }
  }
}


void sobelOperator(ImagePNG *Image, ImagePNG *Source){

  for(int y = 1; y < Image->height-1; y++) {
    png_bytep rowa = Source->row_pointers[y-1];
    png_bytep rowb = Source->row_pointers[y];
    png_bytep rowc = Source->row_pointers[y+1];
    png_bytep row = Image->row_pointers[y];
    for(int x = 1; x < Image->width-1; x++) {
      png_bytep pxaa = &(rowa[(x-1) * 4]);
      png_bytep pxba = &(rowa[(x) * 4]);
      png_bytep pxca = &(rowa[(x+1) * 4]);
      png_bytep pxab = &(rowb[(x-1) * 4]);
      png_bytep pxcb = &(rowb[(x+1) * 4]);
      png_bytep pxac = &(rowc[(x-1) * 4]);
      png_bytep pxbc = &(rowc[(x) * 4]);
      png_bytep pxcc = &(rowc[(x+1) * 4]);
      char gx = -pxaa[0] + pxca[0]
	-2*pxab[0] + 2*pxcb[0]
	-pxac[0] + pxcc[0];
      int gy = -pxaa[0] - 2*pxba[0] - pxca[0]
	+ pxac[0] + 2*pxbc[0] + pxcc[0];

      png_bytep px = &(row[(x) * 4]);
      px[0] = px[1] = px[2] = sqrt(gx*gx+gy*gy); 
    }
  }
}


int main(int argc, char *argv[]) {
  ImagePNG Image;
  ImagePNG Source;

  if(argc != 3) {
    fprintf(stdout,"%s Version %d.%d\n",argv[0], Sixth_VERSION_MAJOR, Sixth_VERSION_MINOR);
    fprintf(stderr, "Usage : %s <inputfile> <outputfile>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  read_png_file(argv[1], &Image);
  //  process_png_file(&Image);
  convertBW(&Image);

  //DEBUG("%s %s %d\n", __FILE__, __func__, __LINE__);


  read_png_file(argv[1], &Source);
  convertBW(&Source);
  sobelOperator(&Image, &Source);

  write_png_file(argv[2], &Image);
  return 0;
}

