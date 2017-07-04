#pragma once

/**
@file DBC.cpp
@brief Clase Differential Box-Counting encapsula el c�lculo de la Dimensi�n Fractal de una imagen

@author Emilio Gallardo Molina
@date 24/04/2017
*/
class DBC
{
private:
	float DF;
	float D;
	float C;
	float E;
	int anchoMatriz;
	int tamListaS;
	interpretacion *grafica;//array  con todos los resultados necesarios para calcular la Dimensi�n fractal

public:
	/**
	@brief calcula la dimensi�n fractal 

	@param array con la imagen cargada en memoria
	@param ancho de la imagen
	@param nivel de gris utilizado para la representaci�n del color
	@param tipo de ejecuci�n
	*/
	DBC(unsigned char *img3, int M,int G,enum opcion miPrograma);
	
	/**
	@brief libera la memoria utilizada por la instacia
	*/
	~DBC(void);
	
	/**
	@brief devuelve el valor del dimensi�n fractal
	@returns DF
	*/
	float getDF(){return DF;};
	
	/**
	@brief realiza todas las operaciones necesarias para realizar el c�lculo objetivo.
	*/
	void calcularDF();//calcula la regresion lineal de la grafica para hallar la pendiente
};

