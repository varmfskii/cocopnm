#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include "pbm.h"
#include "sg4.h"

void help(FILE *, char *);

int main(int argc, char *argv[]) {
  FILE *in = stdin;
  FILE *out = stdout;
  char *inname = NULL;
  char *outname = NULL;
  int color = 7;
  int fmt = BIN;
  int rev = 0;

  pbm *pbm;
  int ix, opt;
  char *sopts = "abc:hi:o:rv";
  struct option lopts[] = {
    { "green", 0, NULL, 0},
    { "yellow", 0, NULL, 1},
    { "blue", 0, NULL, 2},
    { "red", 0, NULL, 3},
    { "white", 0, NULL, 4},
    { "buff", 0, NULL, 4},
    { "cyan", 0, NULL, 5},
    { "magenta", 0, NULL, 6},
    { "orange", 0, NULL, 7},
    { "asm", 0, NULL, 'a'},
    { "bin", 0, NULL, 'b' },
    { "color", 1, NULL, 'c' },
    { "help", 0, NULL, 'h'},
    { "in", 1, NULL, 'i' },
    { "out", 1, NULL, 'o'},
    { "raw", 0, NULL, 'r' },
    { "reverse", 0, NULL, 'v' },
    { NULL, 0, NULL, '\0' }
  };
  while((opt=getopt_long(argc, argv, sopts, lopts, &ix))!=-1) {
    errno = 0;
    if (opt>=0 && opt<=7) {
      color = opt;
      continue;
    }
    switch(opt) {
    case 'a':
      fmt = ASM;
      break;
    case 'b':
      fmt = BIN;
      break;
    case 'c':
      color = strtol(optarg, NULL, 10);
      if (errno || color <0 || color>7) {
	fprintf(stderr, "bad color: %s\n", optarg);
	return -1;
      }
      break;
    case 'h':
      help(stdout, argv[0]);
      break;
    case 'i':
      inname = optarg;
      break;
    case 'o':
      outname = optarg;
      break;
    case 'r':
      fmt = RAW;
      break;
    case 'v':
      rev = !rev;
      break;
    default:
      fprintf(stderr, "Unimplemented option: %c\n", opt);
      help(stderr, argv[0]);
    }
  }
  if (optind<argc && inname==NULL) inname=argv[optind++];
  if (optind<argc && outname==NULL) outname=argv[optind++];
  if (optind<argc) {
    fprintf(stderr, "extraneous arguments\n");
    return -1;
  }
  if (inname && !(in=fopen(inname, "r"))) {
    fprintf(stderr, "unable to read: %s\n", inname);
    return -1;
  }
  pbm = readpbm(in);
  if(outname && !(out=fopen(outname, "w"))) {
    fprintf(stderr, "unable to write: %s\n", outname);
    return -1;
  }
  writesg4(out, pbm, fmt, color, rev);
  return 0;
}
    
void help(FILE *out, char *name) {
  fprintf(out, "Usage: %s <options>\n", name);
  fputs("where options are:\n", out);
  fputs("\t-a\t\t--asm\t\t\tassembly language output\n", out);
  fputs("\t-b\t\t--bin\t\t\tcoco bin file (default)\n", out);
  fputs("\t\t\t--blue\t\t\tblack and blue\n", out);
  fputs("\t\t\t--buff\t\t\tblack and buff (white)\n", out);
  fputs("\t-c<n>\t\t--color <n>\t\tselect color\n", out);
  fputs("\t\t\t--cyan\t\t\tblack and cyan\n", out);
  fputs("\t\t\t--green\t\t\tblack and green\n", out);
  fputs("\t-h\t\t--help\t\t\tshow this help\n", out);
  fputs("\t-i<file>\t--in <file>\t\tinput file name\n", out);
  fputs("\t\t\t--magenta\t\tblack and magenta\n", out);
  fputs("\t\t\t--orange\t\tblack and orange (default)\n", out);
  fputs("\t-o<file>\t--out <file>\t\toutput file name\n", out);
  fputs("\t-r\t\t--raw\t\t\traw binary output\n", out);
  fputs("\t\t\t--red\t\t\tblack and red\n", out);
  fputs("\t-v\t\t--reverse\t\treverse colors\n", out);
  fputs("\t\t\t--white\t\t\tblack and white (buff)\n", out);
  fputs("\t\t\t--yellow\t\tblack and yellow\n", out);
  if (out==stderr) exit(-1);
  exit(0);
}
