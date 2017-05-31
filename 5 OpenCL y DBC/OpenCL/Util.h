#pragma once
#include "stdafx.h"
/*
*Util encapsula todas las funciones necesarias para el resto de clases
*
*/
typedef struct interpretacion{
	//guarda la sumatoria de el valor maximo-minimo+1 para sxs
	float y; 
	//guarda la relacion del tamaño de la muestra s/ancho de la imagen
	float x;
}interpretacion;


typedef struct bmpFileHeader
{
  uint32_t size;
  uint16_t resv1;
  uint16_t resv2;
  uint32_t offset;
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  uint32_t headersize;      /* DIB header size */
  uint32_t width;
  uint32_t height;
  uint16_t planes;         /* color planes */
  uint16_t bpp;            /* bits per pixel */
  uint32_t compress;
  uint32_t imgsize;     
  uint32_t bpmx;        /* X bits per meter */
  uint32_t bpmy;        /* Y bits per meter */
  uint32_t colors;      /* colors used */
  uint32_t imxtcolors;      /* important colors */
} bmpInfoHeader;

int **leerImagenBMP(char *nome,long &ancho,long &alto);

void SaveBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata);



void SaveBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata);
unsigned char calculaColorMedio(unsigned char *pixel);
unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader);
bmpInfoHeader *createInfoHeader(unsigned w, unsigned h, unsigned ppp);

void crearGrafica(interpretacion *grafica, int tam);
void crearMapaZ(int s,float sPrima, int M,float *n);