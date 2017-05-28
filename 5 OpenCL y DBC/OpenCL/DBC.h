#pragma once

//G=nivel de gris(8bpp)
//M=tamaño de la imagen
//s=tamaño del grid M>=s>1
//s'=tamrangoColor=tamaño de cada particion del nivel de gris 
//k= minimo valor en sxs
//l= maximo valor en sxs
//n=l-k+1 en sxs
//N=Sumatoria n
//r=s/ancho comparscion de el tamaño total y el grill seleccionado

/*
*Clase Differential Box-Counting permite calcular la Dimensión Fractal de una imagen
*
*
*
*/
class DBC
{
private:
	float DF;
	float D;
	float C;
	float E;
	int **matriz;//matriz con los valores de una imagen en escala de gris
	int **entradaOpencl;
	int anchoMatriz;
	int numElementos;
	interpretacion *grafica;//array  con todos los resultados necesarios para calcular la Dimensión fractal


	void dividirS( int s);//divide la imagen en grill de tamaño s
	void ordenarS(int s, int I, int J,int &pos);// calcula el valor minimo y maximo de un grill concreto
public:
	DBC(int **matriz, int M,int G);//constructor
	~DBC(void);
	void mostrarGrafica();//muestra por pantalla N sobre 1/r
	float getDF(){return DF;};//devuelve el valor de la dimensión fractal
	void calcularDF();//calcula la regresion lineal de la grafica para hallar la pendiente
};

