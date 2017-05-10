// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//

#pragma once

#include "targetver.h"



//#include <oclUtils.h>



// TODO: mencionar aquí los encabezados adicionales que el programa necesita
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <CL/cl.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)
#define _CRT_SECURE_NO_WARNINGS