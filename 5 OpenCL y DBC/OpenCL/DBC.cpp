#include "stdafx.h"
#include "DBC.h"




DBC::DBC(int **imagen, int ancho,int nivelGris)
{

	anchoMatriz=ancho;
	//inicializar la entrada de opencl
	entradaOpencl =(int **) calloc((anchoMatriz/2-1),sizeof(int *));
	for (int i = 0; i < anchoMatriz/2-1; i++){
		entradaOpencl[i]=(int *) calloc(anchoMatriz*anchoMatriz,sizeof(int));
	}
	entradaOpencl[0][0]=0;

	//tiempo
	clock_t fin,totalInicio;
	totalInicio=clock();

	matriz=imagen;
	anchoMatriz=ancho;
	numElementos=0;

	//guardar los resultado para todos los tama�os de s
	int *v=(int*)calloc(anchoMatriz/2,sizeof(int));

	//halla N para tama�os M>=s>1d
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		DBC::dividirS(s);
		fin=clock();
	}

	printf("\ntiempo total de la ordenacion: %f segundos\n",(fin-totalInicio)/(double)CLOCKS_PER_SEC);

	//llamar al programa
	shader s=shader();
	float f=s.getDF(entradaOpencl,anchoMatriz,2);
	f=s.getDF(entradaOpencl,anchoMatriz,4);
	

}


DBC::~DBC(void)
{
	for (int i = 0; i < anchoMatriz; i++)
	{
		free(matriz[i]);
	}

	free(matriz);
}

//ordena las cadillas de los grid en la fila de la matriz con los valores de entrada
void DBC::ordenarS(int s, int I, int J,int &pos){

	//corrigiendo el desbordamiento 
	if(I>anchoMatriz-s)
		I=anchoMatriz-s;
	if(J>anchoMatriz-s)
		J=anchoMatriz-s;

	for (int i = I; i < I+s; i++)
	{
		for (int j = J; j  < J+s; j++)
		{

			entradaOpencl[s-2][pos]=matriz[i][j];
			pos+=1;
		}
	}

}

//Calcula N para un tama�o s dado
void DBC::dividirS(int s){

	int pos=0;
	for (int i = 0; i < anchoMatriz/s*s; i+=s)
	{
		for (int j = 0; j  < anchoMatriz/s*s; j+=s)
		{
			DBC::ordenarS(s, i, j,pos);
		}

	}
}

void DBC::mostrarGrafica(){


}


void DBC::calcularDF(){
	/*
	float penxy=0.0;
	float sumax=0.0;
	float sumay=0.0;
	float sumx2=0.0;
	float sumy2=0.0;

	int tam=numElementos;

	//inicia la parte de regresion lineal

	//x=log(1/r)
	//y=log(N)


	float a=tam*penxy;
	float b=sumax*sumay;
	float c=tam*sumx2;
	float d=pow(sumax,2);

	D=(a-b)/(c-d);
	printf("\ny=D*x+C\n");
	printf("Pendiente = D = %f\n",D);

	float e=(sumay*(1/(float)tam));
	float f=(D*(1/(float)tam)*sumax);

	C=e-f;


	//calculo del error
	float sumae=0;
	float divisor=0;

	for(int i=0;i<tam;i++){
	sumae+=pow(divisor,2)/(1+pow(D,2));

	}

	printf("\nC=%f\n",C);
	E=sqrt(sumae)/(float)tam;
	printf("E=%f\n",E);*/


}

