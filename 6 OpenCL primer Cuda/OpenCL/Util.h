#pragma once
#include "stdafx.h"

//configurar app
static char *path=".\\images\\D33.bmp";
static bool mostrarMapaZ=false;//mapa de profindidades
static bool mostrarGafica=true;//grafica con la pendiente generada por DF
static bool mostrarTabla=true;//muestra los resultados de D para cualquier s
static bool mostrarDepuracion=true;//informacion de opencl: sPrima, global size y localside
static bool mostrarMinMax=false;//calculos resumidos del valo minimo y maximo de un grid para todos los s
static int plataforma=2;//0=intel-1.0; 1=intel 1.2; 2=cuda
static bool ejecutarOpenCL=true;
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

float CalcularNenC(int *vEntrada, int M,int s);