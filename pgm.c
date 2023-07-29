/************************************************************************/
/* PGM-READER-INVERTER                                                  */
/* ===================                                                  */
/*                                                                      */
/* This program is a work for my Image Recognition course. Reading a    */
/* PGM file correctly, simply inverting and saving it to another file.  */
/* It is the basic necessary tool for Image Recognition.                */
/*                                                                      */
/* It reads both the type 2 (text) and 5 (binary) gray valued PGM       */
/* files.                                                               */
/*                                                                      */
/* PGM files are thought to be the standart for portable graphics       */
/*                                                                      */
/* This program is free software. You can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation.                                        */
/*                                                                      */
/* By Ergin ALTINTAS - ergin@altintas.org                               */
/* https://ergin.altintas.org                                           */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCOLOR 	256

typedef struct {
  unsigned short int	type;
  unsigned int	width,height;
  unsigned int 	maxval;
  unsigned char		*pixel;
} PGM;

void load_pgm(char *filename, PGM *pgm)
{
  FILE    		*inf;
  char 		ch;
  char 		str[200];
  int 		i, j, value[3], val;
  char		c1, c2;
  unsigned char 		*ptr;

  inf = fopen(filename, "rb");
  if (inf == NULL) {
    printf("No such file!n");
    exit(1);
  }
  else {
    fscanf(inf, "%c%c", &c1, &c2);
    if (c1 != 'P' || (c2 != '2' && c2 != '5')) {
      printf("Not a supported PGM file type!n");
      fclose(inf);
      exit(1);
    }
  }

  for (j=0; j<3; j++) {
  do {
    i=0;
    ch = fgetc(inf);
    if (ch == '#') do {
      str[i++]=fgetc(inf);
    } while (str[i-1]!= 'n');
    str[i]=0;
//    printf("COMMENT: %s",str); //*** if wished comments can be printed out here
  } while(ch < '0' || ch > '9');

  i = 0;
  str[i++] = ch;
    do {
      ch = fgetc(inf);
      str[i++] = ch;
    } while(ch >= '0' && ch <= '9');
    str[i-1] = 0;
    value[j] = atoi(str);
    i=0;
  }

  pgm->type   = c2-'0';
  pgm->width  = value[0];
  pgm->height = value[1];
  pgm->maxval = value[2];
  pgm->pixel  = (unsigned char *)malloc (pgm->height*pgm->width);

  if (pgm->pixel == NULL) {
   printf("MALLOC ERROR");
   exit(1);
  }

  switch (pgm->type) {
    case 2:
      ptr=pgm->pixel;
      for (i=0; i < pgm->height; i++) {
	for (j=0; j < pgm->width; j++) {
	  fscanf(inf,"%i",&val);
	  *ptr++=val;
	}
      }
    break;

    case 5:
      ptr=pgm->pixel;
      for (i=0; i < pgm->height; i++) {
	for (j=0; j < pgm->width; j++) {
	  fscanf(inf,"%c",&ch);
	  *ptr++=ch;
//  if eof(inf) { printf("Corrupt PGM file."); exit(1); } // not necessary
	}
      }

    break;
  }

  fclose(inf);
}


void save_pgm(char *filename, PGM pgm)
{
  FILE		*outf;
  int 		i, j;
  unsigned char 		*ptr;

  outf = fopen(filename, "wt");
   if (outf == NULL) {
    printf("No such file!n");
    exit(1);
  }
  fprintf(outf,"P%dxD",pgm.type); // xD s are linefeeds of unix format, 
//'n' creates problem when runing under windows etc.
  fprintf(outf,"%i %ixD", pgm.width, pgm.height);
  fprintf(outf,"%ixD", pgm.maxval);

  switch (pgm.type) {
    case 2:
      ptr=pgm.pixel;
      for (i=0; i < pgm.height; i++) {
	for (j=0; j < pgm.width; j++) {
	  fprintf(outf,"%u ",*ptr++);
	}
	fprintf(outf,"n");
      }
    break;

    case 5:
      fwrite(pgm.pixel, pgm.height, pgm.width, outf);
    break;
  }
  fclose(outf);
}


void display_pgm(PGM pgm)
{
  int i, j;
  unsigned char* ptr;

  printf ("nPGM type:%d width:%d height:%d maxval:%dn", pgm.type, pgm.width, pgm.height, pgm.maxval);
  ptr=pgm.pixel;
  for (i=0; i < pgm.height; i++) {
    for (j=0; j < pgm.width; j++) {
      printf ("%3d",*ptr++);
    }
    printf ("n");
  }
}


void invert_pgm(PGM *pgm)
{
  int i, j;
  unsigned char* ptr, a;

  ptr=pgm->pixel;
  for (i=0; i < pgm->height; i++) {
    for (j=0; j < pgm->width; j++) {
      a=pgm->maxval-*ptr;
      *ptr++=a;
    }
  }
}


int main(int argc, char *argv[])
{
  PGM		pgm1;
  Histogram 	hist, sum;
  Histogram 	normhist;
  char		filename[256];

  printf("nSource PGM filename to read: "); scanf("%s", filename);
  load_pgm(filename, &pgm1);
//  display_pgm(pgm1); // if you want to see the source
  invert_pgm(&pgm1);
//  display_pgm(pgm1); // if you want to see the source
  printf("nInverted PGM filename to write: "); scanf("%s", filename);
  save_pgm(filename, pgm1);

  return EXIT_SUCCESS;
}
