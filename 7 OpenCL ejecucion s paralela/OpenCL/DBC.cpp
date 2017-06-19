#include "stdafx.h"
#include "DBC.h"




DBC::DBC(unsigned char *img3, int ancho,int nivelGris,enum opcion miPrograma)
{

	float N=0;
	float r=0;
	anchoMatriz=ancho;
	//inicializar la entrada de opencl
	grafica=(interpretacion*) calloc(anchoMatriz/2-1,sizeof(interpretacion));
	float *NdeS=(float*) calloc(anchoMatriz/2-1,sizeof(float));

	//numero de divisores de M/2
	tamListaS=0;
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		if(anchoMatriz%s==0)
			tamListaS+=1;
	}
	int *listaS=(int*) calloc(tamListaS,sizeof(int));
	int pos=0;
	for (int s = 2; s <= anchoMatriz/2; s++)
	{
		if(anchoMatriz%s==0){
			listaS[pos]=s;
			//printf("s=%i\n",s);
			pos+=1;
		}
	}

	//tiempo
	clock_t fin,totalInicio;
	//shaderGPU progGPU=shaderGPU();
	shaderCPU progCPU=shaderCPU();
	shaderGPU progGPU=shaderGPU();
	printf("cargando programa %i\n",miPrograma);
	switch (miPrograma)
	{
	case _C:
		
		for (int i = 0; i < tamListaS; i++)
		{
			grafica[i].y=log10(CalcularNenC(img3, anchoMatriz,listaS[i]));
		}
		break;
	case _CPU:
		progCPU.CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log10(NdeS[i]);
		}
		break;
	case _GPU:
		progGPU.CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log10(NdeS[i]);
		}
		break;
	default:
		break;
	}

	//mostrar resultado de la ejecucion
	if(mostrarTabla)printf("s\t;N\t;r\t;logN\t;lor(1/r)\t; \n");
	for (int i = 0; i <tamListaS; i++)
	{
		r=listaS[i]/(float)anchoMatriz;
		//r=(float)anchoMatriz/(float)s;
		grafica[i].x=log10(1/r);
		if(mostrarTabla)printf("%i\t;%f\t;%f\t;%f\t;%f\n",listaS[i],pow(10,grafica[i].y),1/pow(10,grafica[i].x),grafica[i].y,grafica[i].x);
	}
	//crear gr�fica
	if(mostrarGafica)
		crearGrafica(grafica,tamListaS);

	calcularDF();

	free(listaS);
	free(NdeS);
}


DBC::~DBC(void)
{


	free(grafica);
}


void DBC::calcularDF(){

	float penxy=0.0;
	float sumax=0.0;
	float sumay=0.0;
	float sumx2=0.0;
	float sumy2=0.0;

	int tam=tamListaS;

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
	//printf("\ny=D*x+C\n");
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

	//printf("\nC=%f\n",C);
	E=sqrt(sumae)/(float)tam;
	printf("E=%f\n",E);

}

