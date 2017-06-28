// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//

#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <CL/cl.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_NOMARCH 256


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"
#include "Util.h"
#include "DBC.h"
#include "shader.h"
#include "shaderCPU.h"
#include "shaderCPU_superficie.h"
#include "shaderGPU.h"
#include "shaderGPU2_0_0.h"
#include "shaderGPU2_0_1.h"
#include "shaderGPU2_1.h"
#include "shaderGPU2_2.h"




