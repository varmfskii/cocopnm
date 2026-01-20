#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pbm.h"

char getpixel(FILE *in);
void getblank(FILE *in);

pbm *readpbm(FILE *in) {
  pbm *pbm = malloc(sizeof(pbm));
  int w, h, rv;
  char **raster;
  
  if (!pbm) {
    fprintf(stderr, "allocation failed\n");
    exit(-1);
  }
  
  if (getc(in)!='P' || getc(in)!='1' || !isspace(getc(in))) {
    fprintf(stderr, "Bad magic number\n");
    exit(-1);
  }
  getblank(in);
  rv=fscanf(in, "%d%d", &w, &h);
  if (rv!=2) {
    fprintf(stderr, "Malformed file\n");
    exit(-1);
  }
  pbm->w = w;
  pbm->h = h;
  getblank(in);
  // space to hold image
  if (!(raster=malloc(sizeof(char *)*h))) {
    fprintf(stderr, "allocation error\n");
    exit(-1);
  }
  for(int i=0; i<h; i++)
    if (!(raster[i]=malloc(w))) {
      fprintf(stderr, "allocation error\n");
      exit(-1);
    }
  for (int i=0; i<h; i++)
    for (int j=0; j<w; j++)
      raster[i][j] = getpixel(in);
  pbm->raster = raster;
  return pbm;
}

void getblank(FILE *in) {
  int c;
  while(1) {
    while(isspace(c=getc(in)));
    if (c!='#') {
      ungetc(c, in);
      return;
    }
    while(c!=EOF && c!='\n' && c!='\r') c=getc(in);
  }
}

char getpixel(FILE *in) {
  int c;
  while(isspace(c=getc(in)));
  switch(c) {
  case '0':
    return 0;
  case '1':
    return 1;
  case EOF:
    fprintf(stderr, "premature end of file\n");
    exit(-1);
  default:
    fprintf(stderr, "unexpected data\n");
    exit(-1);
  }
}
