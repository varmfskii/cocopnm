#ifndef PBM_H
#define PBM_H
#include <stdio.h>

typedef struct pbm {
  int w, h;
  char **raster;
} pbm;

pbm *readpbm(FILE *in);
#endif
