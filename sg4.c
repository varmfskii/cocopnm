#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pbm.h"
#include "sg4.h"

void writesg4(FILE *out, pbm *pbm, int fmt, int color, int rev) {
  char scn[512];
  if (pbm->w>64 || pbm->h>48) {
    fprintf(stderr, "Image too large\n");
    exit(-1);
  }
  if (rev) {
    memset(scn, color*16+0x80, 512);
    for(int i=0; i<pbm->h; i+=2) {
      for(int j=0, ix=(i/2)*32; j<pbm->w; j+=2, ix++) {
	if(pbm->raster[i][j]) scn[ix]|=0x08;
	if(j+1<pbm->w && pbm->raster[i][j+1]) scn[ix]|=0x04;
	if(i+1<pbm->h && pbm->raster[i+1][j]) scn[ix]|=0x02;
	if(i+1<pbm->h && j+1<pbm->w && pbm->raster[i+1][j+1]) scn[ix]|=0x01;
      }
    }
  } else {
    memset(scn, color*16+0x8f, 512);
    for(int i=0; i<pbm->h; i+=2) {
      for(int j=0, ix=(i/2)*32; j<pbm->w; j+=2, ix++) {
	if(pbm->raster[i][j]) scn[ix]&=0xf7;
	if(j+1<pbm->w && pbm->raster[i][j+1]) scn[ix]&=0xfb;
	if(i+1<pbm->h && pbm->raster[i+1][j]) scn[ix]&=0xfd;
	if(i+1<pbm->h && j+1<pbm->w && pbm->raster[i+1][j+1]) scn[ix]&=0xfe;
      }
    }
  }
  // write
  if (fmt==ASM) {
    for(int i=0; i<512; i+=16) {
      fprintf(out, "\tfcb $%02x", scn[i]&0xff);
      for(int j=1; j<16; j++) fprintf(out, ",$%02x", scn[i+j]&0xff);
      putc('\n', out);
    }
    return;
  }
  if (fmt==BIN) {
    // header
    putc(0,out); // block
    putc(2,out); // size 0x0200
    putc(0,out);
    putc(4,out); // address 0x0400
    putc(0,out);
  }
  fwrite(scn, 1, 512, out);
  if (fmt==BIN) {
    // footer
    putc(0xff,out); // end
    putc(0,out); // 0x0000
    putc(0,out);
    putc(0,out); // start 0x0000
    putc(0,out);
    fclose(out);
  }
}
