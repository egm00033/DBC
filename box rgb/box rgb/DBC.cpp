#include "stdafx.h"
#include "DBC.h"
#include <time.h>


DBC::DBC(int **imagen, int ancho,int nivelGris)
{
	//tiempo
	clock_t inicio,fin,totalInicio;
	totalInicio=inicio=clock();

	matriz=imagen;
	float sPrima;
	anchoMatriz=ancho;
	numElementos=0;

	//guardar los resultado para todos los tamaños de s
	grafica=(interpretacion*) calloc(anchoMatriz/2-1,sizeof(interpretacion));
	int *v=(int*)calloc(anchoMatriz/2,sizeof(int));

	for (int i = 0; i < anchoMatriz/2-1; i++)
	{
		grafica[i].y=0;
		grafica[i].x=0;
	}


	printf("s\t;r\t;N\t;y\t;x\t;D\n");
	//halla N para tamaños M>=s>1d
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		float r=(float)s/(float)anchoMatriz;
		sPrima=(float)nivelGris*(float)s/(float)anchoMatriz;
		int N=DBC::calcularN(s,sPrima);

		float y=log10((float)N);
		float x=log10(1/r);
		float D=(y/x);

		//se guarda en una estructura para el cálculo de D, C y E.
		grafica[numElementos].y=y;
		grafica[numElementos].x=x;
		numElementos+=1;
		fin=clock();
		if(anchoMatriz%s==0){
			printf("%i\t;%f\t;%i\t;%f\t;%f\t;%f\n",s,r,N,y,x,D);
		}
		inicio=clock();


		//}
	}

	printf("\ntiempo total de ejecucion: %f segundos\n",(fin-totalInicio)/(double)CLOCKS_PER_SEC);

	calcularDF();


}


DBC::~DBC(void)
{
	for (int i = 0; i < anchoMatriz; i++)
	{
		free(matriz[i]);
	}

	free(matriz);
	free(grafica);
}

//devuelve el valor n de un grid sxs
int DBC::calculars(int s, int I, int J, float sPrima){
	int k=257;
	int l=0;
	int n=0;

	//corrigiendo el desbordamiento 
	if(I>anchoMatriz-s)
		I=anchoMatriz-s;
	if(J>anchoMatriz-s)
		J=anchoMatriz-s;

	for (int i = I; i < I+s; i++)
	{
		for (int j = J; j  < J+s; j++)
		{
			if(matriz[i][j]<k){
				k=matriz[i][j];
			}
			if(matriz[i][j]>l){
				l=matriz[i][j];
			}
		}
	}

	l=(int)((float)l/(float)(sPrima));
	k=(int)((float)k/(float)(sPrima));
	n=((int)((float)l/(float)(sPrima))-((float)k/(float)(sPrima)))+1;
	/*if(s==320){
	printf(" n= %i s= %i \n",n,s);
	}*/
	return n;
}

//Calcula N para un tamaño s dado
int DBC::calcularN(int s,float sPrima){
	int N=0;
	for (int i = 0; i < anchoMatriz; i+=s)
	{
		for (int j = 0; j  < anchoMatriz; j+=s)
		{
			N+=DBC::calculars(s, i, j,sPrima);
		}

	}
	return N;
}

void DBC::mostrarGrafica(){
	printf("\n valores leidos de la imagen\nlog(N);log(1/r) \n\n");
	for (int i = numElementos-1; i >= 0; i--)
	{	

		printf("%f;%f;\n", grafica[i].y, grafica[i].x);
	}

}


void DBC::calcularDF(){

	float penxy=0.0;
	float sumax=0.0;
	float sumay=0.0;
	float sumx2=0.0;
	float sumy2=0.0;

	int tam=numElementos;

	//inicia la parte de regresion lineal

	//x=log(1/r)
	//y=log(N)
	for(int i=0;i<tam;i++){
		sumay+=grafica[i].y;
		sumax+=grafica[i].x;
		sumy2+=grafica[i].y*grafica[i].y;
		sumx2+=grafica[i].x*grafica[i].x;
		penxy+=grafica[i].y*grafica[i].x;
	}

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
		divisor=(D*grafica[i].x+C-grafica[i].y);
		sumae+=pow(divisor,2)/(1+pow(D,2));

	}

	printf("\nC=%f\n",C);
	E=sqrt(sumae)/(float)tam;
	printf("E=%f\n",E);

	//mostrar solo si el error es menor a E
	/*printf("\n Error menor a %f\nlog(N);log(1/r) \n\n",E);
	float errorPunto;
	for (int i = numElementos-1; i >= 0; i--)
	{	
		errorPunto=grafica[i].y-grafica[i].x*D-C;
		if(errorPunto<=E&&errorPunto>(-E)){
			printf("%f;%f;\n", grafica[i].y, grafica[i].x);
		}
	}*/
}
