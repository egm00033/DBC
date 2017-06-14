#include "stdafx.h"
#include "DBC.h"




DBC::DBC(unsigned char *img3, int ancho,int nivelGris)
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
			pos+=1;
		}
	}
	for (int i = 0; i < tamListaS; i++)
	{
		printf("%i : s=%i\n",i,listaS[i]);
	}

	//tiempo
	clock_t fin,totalInicio;
	shaderGPU progGPU=shaderGPU();
	shaderCPU progCPU=shaderCPU();

	switch (miPrograma)
	{
	case _C:
		for (int i = 0; i < tamListaS; i++)
		{
			grafica[i].y=log10(CalcularNenC(img3, anchoMatriz,listaS[i]));
		}
		break;
	case _CPU:
		progCPU.CalcularN(img3,NdeS,anchoMatriz,tamListaS);
		for(int i=0; i < tamListaS; i++){
			grafica[i].y=log10(NdeS[i]);
		}
		break;
	case _GPU:
		//llamar al programa

		if(progGPU.getRet()==0){//creacion con éxito
			totalInicio=clock();
			//calcular en OpenCL
			progGPU.CalcularN(img3,NdeS,anchoMatriz);
			fin=clock();
			for(int s = 2; s <= anchoMatriz/2; s++){
				//printf("valor de NdeS[%i]=%f\n",s,NdeS[s-2]);
				grafica[s-2].y=NdeS[s-2];
			}

			printf("\ntiempo de la ejecucion del shader: %f segundos\n",(fin-totalInicio)/(double)CLOCKS_PER_SEC);




			//printf("%f\t%f\t%f\t%f\t%f\t%f\t\n",D,E,pow(10,grafica[318].y),grafica[318].y  ,pow(10,grafica[0].y),grafica[0].y);
		}else{
			printf("ERROR ret==0\n");

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
	//crear gráfica
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

