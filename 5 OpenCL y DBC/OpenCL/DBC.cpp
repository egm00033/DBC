#include "stdafx.h"
#include "DBC.h"




DBC::DBC(int **imagen, int ancho,int nivelGris)
{
	printf("DBC\n");
	anchoMatriz=ancho;
	//inicializar la entrada de opencl
	grafica=(interpretacion*) calloc(anchoMatriz/2-1,sizeof(interpretacion));
	entradaOpencl =(int **) calloc((anchoMatriz/2-1),sizeof(int *));
	for (int i = 0; i < anchoMatriz/2-1; i++){
		entradaOpencl[i]=(int *) calloc(anchoMatriz*anchoMatriz,sizeof(int));
	}
	entradaOpencl[0][0]=0;

	//tiempo
	clock_t fin,totalInicio;


	matriz=imagen;
	anchoMatriz=ancho;

	//guardar los resultado para todos los tamaños de s
	int *v=(int*)calloc(anchoMatriz/2,sizeof(int));

	//halla N para tamaños M>=s>1d
	totalInicio=clock();
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		DBC::dividirS(s);
		//printf("subdividiendo s= %i\n",s);
	}
	fin=clock();
	printf("\ntiempo total de la ordenacion: %f segundos\n",(fin-totalInicio)/(double)CLOCKS_PER_SEC);

	//llamar al programa
	shader programa=shader();
	float N=0;


	float r=0;

	totalInicio=clock();
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		N=programa.getDF(entradaOpencl[s-2],anchoMatriz/s*s,s);
		grafica[s-2].y=log10((float)N);
	}
	fin=clock();	
	printf("\ntiempo de la ejecucion del shader: %f segundos\n",(fin-totalInicio)/(double)CLOCKS_PER_SEC);

	printf("s; N ; r ;logN;lor(1/r)\n");
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		r=(float)s/(float)anchoMatriz;
		grafica[s-2].x=log10(1/r);
		printf("%i;%f;%f;\n",s,grafica[s-2].y,grafica[s-2].x);
	}

	for (int i = 0; i < anchoMatriz/2-1; i++)
	{
		free(entradaOpencl[i]);
	}
	free(entradaOpencl);
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
			//printf("%i\t",matriz[i][j]);
			pos+=1;

		}
		//printf("\n");
	}

}

//Calcula N para un tamaño s dado
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

