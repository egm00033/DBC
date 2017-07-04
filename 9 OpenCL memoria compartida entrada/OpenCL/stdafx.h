/**
@file stdafx.h
@brief archivo de inclusión de los archivos de inclusión estándar del sistema
o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
pero rara vez modificados 

@author Emilio Gallardo Molina
@date 24/04/2017
*/
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



#include <CL/cl.h>
#define MAX_SOURCE_SIZE (0x100000)
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"
#include "Util.h"
#include "DBC.h"
#include "shaders\shader.h"
#include "shaders\shaderCPU.h"
#include "shaders\shaderCPU_superficie.h"
#include "shaders\shaderGPU.h"
#include "shaders\shaderGPU2_0_0.h"
#include "shaders\shaderGPU2_0_1.h"
#include "shaders\shaderGPU2_1.h"
#include "shaders\shaderGPU2_2.h"




