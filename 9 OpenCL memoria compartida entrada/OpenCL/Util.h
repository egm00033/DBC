#pragma once
#include "stdafx.h"

/**
  @file Util.h
  @brief define todas las variables y funciones auxiliares que necesita la aplicación 

  @author Emilio Gallardo Molina
  @date 24/04/2017
*/

static char *path=".\\images\\D3.bmp";//ruta de la imagen
static bool mostrarMapaZ=false;//mapa de profindidades
static bool mostrarGafica=false;//grafica con la pendiente generada por DF
static bool mostrarTabla=false;//muestra los resultados de D para cualquier s
static bool mostrarDepuracion=false;//informacion de opencl: sPrima, global size y localside
static bool mostrarMinMax=false;//calculos resumidos del valo minimo y maximo de un grid para todos los s
static bool divisoresPares=true;//se ejecuta con los divisores de 128 hasta 2 o con los de 320 hasta 5

/**
  @brief enumeracióntipos de shader
  */
enum opcion {_C, _CPU_profundidad, _CPU_superficie, _GPU1, _GPU2_0_0, _GPU2_0_1, _GPU2_1, _GPU2_2};

/**
@brief guarda el valor 'x' e 'y' de un punto para la representación del cálculo diferencial
*/
typedef struct interpretacion{
	//guarda la sumatoria de el valor maximo-minimo+1 para sxs
	float y; 
	//guarda la relacion del tamaño de la muestra s/ancho de la imagen
	float x;
}interpretacion;

/**
@brief datos del archivo a calcular
*/
typedef struct bmpFileHeader
{
  uint32_t size;
  uint16_t resv1;
  uint16_t resv2;
  uint32_t offset;
} bmpFileHeader;

/**
@brief información de la imagen cargada
*/
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

/**
  @brief carga la imagen en memoria

  @param ruta del archivo bmp
  @param rellena la información de interés de la imagen

  @returns array con el valor de cada pixel
 */
unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader);
/**
  @brief guarda una imagen con los puntos obtenido del cálculo de la dimensión fractal para un tamaño de r dado

  @param array de cada punto con el valor (x,y) y el número de puntos calculados
  @param rellena la información de interés de la imagen
 */
void crearGrafica(interpretacion *grafica, int tam);

/**
  @brief muestra un mapa de profundidades representativo, cada pixel toma el valor n(maximo-minimo) del grid al que pertenece

  @param tamaño del grid
  @param valor de cada bloque z de la imagen
  @param tamaño de la imagen
  @param array con la profundidad de cada pixel
 */
void crearMapaZ(int s,float sPrima, int M,float *n);

/**
  @brief lee los metadatos de una imagen

  @param ancho de la imagen
  @param alto de la imagen
  @param puntos por pixels

  @returns metadatos de la imagen
 */
bmpInfoHeader *createInfoHeader(unsigned w, unsigned h, unsigned ppp);

/**
  @brief calcula el valor N de una imagen dada para un tamaño de grid sxs

  @param imagen cargada en memoria
  @param tamaño de la imagen
  @param tamaño del grid

  @returns sumatoria del valor n=(maximo-minimo) de cada grid
*/
float CalcularNenC(unsigned char *vEntrada, int M,int s);

/**
  @brief es utilizado por crearMapaZ para almacenar la imagen

  @param nombre
  @param metadatos
  @param datos de cada pixel
*/
void SaveBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata);







