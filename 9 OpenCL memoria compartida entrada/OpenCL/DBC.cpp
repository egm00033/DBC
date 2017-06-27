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
	int pos=0;
	//numero de divisores de M/2
	tamListaS=0;
	int sInicial=2;
	int *listaS;
	if(divisoresPares){
		sInicial=2;
		for (int s = sInicial; s <= anchoMatriz/2; s*=2)
		{
			if(anchoMatriz%s==0)
				tamListaS+=1;
		}
		listaS=(int*) calloc(tamListaS,sizeof(int));

		for (int s = sInicial; s <= anchoMatriz/2; s*=2)
		{
			if(anchoMatriz%s==0){
				listaS[pos]=s;
				//printf("s=%i\n",s);
				pos+=1;
			}
		}
	}else{
		sInicial=5;
		for (int s = sInicial; s <= anchoMatriz/2; s*=2)
		{
			if(anchoMatriz%s==0)
				tamListaS+=1;
		}
		listaS=(int*) calloc(tamListaS,sizeof(int));

		for (int s = sInicial; s <= anchoMatriz/2; s*=2)
		{
			if(anchoMatriz%s==0){
				listaS[pos]=s;
				//printf("s=%i\n",s);
				pos+=1;
			}
		}

	}


	//tiempo
	clock_t inicio;

	shaderCPU *progCPUp=NULL;

	shaderCPU_superficie *progCPUs=NULL;

	shaderGPU *progGPU=NULL;
	shaderGPU2_0 *progGPU2_0=NULL;
	shaderGPU2_1 *progGPU2_1=NULL;

	printf("cargando programa %i\n",miPrograma);

	switch (miPrograma)
	{
	case _C:
		printf("Ejecutando en c \n");
		inicio=clock();
		for (int i = 0; i < tamListaS; i++)
		{
			grafica[i].y=(CalcularNenC(img3, anchoMatriz,listaS[i]));
		}
		printf("Tiempo de ejecucion: %f segundoss, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for (int i = 0; i < tamListaS; i++)
		{
			grafica[i].y=log(grafica[i].y)/ log( 2.0 );
		}

		break;
	case _CPU_profundidad:
		progCPUp=new shaderCPU();
		printf("Ejecutando en CPU profundidad\n");
		inicio=clock();
		progCPUp->CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		printf("Tiempo de ejecucion: %f segundos, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log(NdeS[i])/ log( 2.0 );
		}
		break;
	case _CPU_superficie:
		progCPUs=new shaderCPU_superficie();
		printf("Ejecutando en CPU por superficie\n");
		inicio=clock();
		progCPUs->CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		printf("Tiempo de ejecucion: %f segundos, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log(NdeS[i])/ log( 2.0 );
		}
		break;
	case _GPU:
		progGPU=new shaderGPU();

		printf("Ejecutando en GPU \n");
		inicio=clock();
		progGPU->CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		printf("Tiempo de ejecucion: %f segundos, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log(NdeS[i])/ log( 2.0 );
		}
		break;
	case _GPU2_0:

		progGPU2_0=new shaderGPU2_0();
		printf("Ejecutando en GPU2_0 2D\n");
		inicio=clock();
		progGPU2_0->CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		printf("Tiempo de ejecucion: %f segundos, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log(NdeS[i])/ log( 2.0 );
		}
		break;
	case _GPU2_1:

		progGPU2_1=new shaderGPU2_1();
		printf("Ejecutando en GPU2_1 memoria compartida \n");
		inicio=clock();
		progGPU2_1->CalcularN(img3,NdeS,anchoMatriz,tamListaS,listaS);
		printf("Tiempo de ejecucion: %f segundos, clocks=%i \n",(clock()-inicio)/(double)CLOCKS_PER_SEC,clock()-inicio);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log(NdeS[i])/ log( 2.0 );
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
		grafica[i].x=log(1/r)/ log( 2.0 );
		if(mostrarTabla)printf("%i\t;%f\t;%f\t;%f\t;%f\n",listaS[i],pow(10,grafica[i].y),1/pow(10,grafica[i].x),grafica[i].y,grafica[i].x);
	}
	//crear gráfica
	if(mostrarGafica)
		crearGrafica(grafica,tamListaS);

	calcularDF();
	free(listaS);
	free(NdeS);

	if(progCPUp!=NULL)delete progCPUp;
	if(progCPUs!=NULL)delete progCPUs;
	if(progGPU!=NULL)delete progGPU;
	if(progGPU2_0!=NULL)delete progGPU2_0;

	printf("\n");
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

