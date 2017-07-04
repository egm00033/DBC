#include "stdafx.h"
#include "Util.h"




void crearGrafica(interpretacion *grafica, int tam){
	bmpInfoHeader info;  
	unsigned char *img;
	unsigned char color[3];
	unsigned char blanco;
	int i, j;
	img=LoadBMP(".\\images\\grafica.bmp", &info);

	for (i=0; i<info.height; i++)
	{
		for (j=0; j<info.width; j++)
		{
			blanco=255;
			img[3*(j+i*info.width)]=blanco;
			img[3*(j+i*info.width)+1]=blanco;
			img[3*(j+i*info.width)+2]=blanco;
		}
	}
	//buscar el valor mayor para ponderar la gráfica con el tamaño de la imagen
	float max=0;
	for(i=0; i<tam; i++){
		if(max<grafica[i].y)max=grafica[i].y;
	}
	//pintar grafica
	max+=0.5;//margen
	int posx,posy;
	for(i=0; i<tam; i++){
		posx = (int)(grafica[i].x/max*info.width);
		posy = (int)(grafica[i].y/max*info.width);
		//printf("%i \n",posx);
		img[3*(posx+posy*info.width)]=255;
		img[3*(posx+posy*info.width)+1]=0;
		img[3*(posx+posy*info.width)+2]=0;
	}

	SaveBMP(".\\images\\mapaZ\\grafica.bmp", &info, img);
	system (".\\images\\mapaZ\\grafica.bmp");

	free(img);

}

void crearMapaZ(int s,float sPrima, int M,float *n){
	bmpInfoHeader info;  
	unsigned char *img;
	unsigned char blanco;
	int i, j;
	img=LoadBMP(".\\images\\grafica.bmp", &info);

	for (i=0; i<info.height; i++)
	{
		for (j=0; j<info.width; j++)
		{
			blanco=255;
			img[3*(j+i*info.width)]=blanco;
			img[3*(j+i*info.width)+1]=blanco;
			img[3*(j+i*info.width)+2]=blanco;
		}
	}

	int ngrid=pow(M/s,2.0);
	int ancho=M/s;
	printf("n grid = %i,ancho = %i,  s=%i, sprima=%f\n",ngrid,M/s,s, sPrima);
	for (i=0; i<ngrid; i++)
	{
		int color=(n[i]-1)*sPrima;
		for (int j = 0; j  < s; j ++)
		{
			for (int k = 0; k < s; k++)
			{
				//prueba para comprobar que pinta bien los grid=OK
				/*int pos=i/ancho*M*s+i%ancho*s+j*M+k;*/

				int pos=i/ancho*M*s+i%ancho*s+j*M+k;
				//printf("%i\t",pos);
				img[pos*3+0]=(color);
				img[pos*3+1]=(color);
				img[pos*3+2]=(color);
			}
			//printf("\n");
		}
		//system("pause");
	}



	SaveBMP(".\\images\\mapaZ\\imagenZ.bmp", &info, img);
	system (".\\images\\mapaZ\\imagenZ.bmp");
	system("pause");


	free(img);

}

unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader)
{
	FILE *f;
	bmpFileHeader header;
	unsigned char *imgdata;
	uint16_t type;
	f=fopen (filename, "r");
	/* handle open error */
	fread(&type, sizeof(uint16_t), 1, f);
	if (type !=0x4D42)
	{
		fclose(f);
		return NULL;
	}
	fread(&header, sizeof(bmpFileHeader), 1, f);
	fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);
	if(mostrarDepuracion){
		printf ("size: %u\n", header.size);
		printf ("offs: %u\n", header.offset);
		printf ("header size:      %d\n", bInfoHeader->headersize);
		printf ("image width:      %d\n", bInfoHeader->width);
		printf ("image height:     %d\n", bInfoHeader->height);
		printf ("colour planes:    %d\n", bInfoHeader->planes);
		printf ("bpp:              %d\n", bInfoHeader->bpp);
		printf ("compress:         %d\n", bInfoHeader->compress);
		printf ("imgage size:      %d\n", bInfoHeader->imgsize);
		printf ("bpmx:             %d\n", bInfoHeader->bpmx);
		printf ("bpmy:             %d\n", bInfoHeader->bpmy);
		printf ("colors:           %d\n", bInfoHeader->colors);
		printf ("important colors: %d\n", bInfoHeader->imxtcolors);
	}
	imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);
	fseek(f, header.offset, SEEK_SET);
	fread(imgdata, bInfoHeader->imgsize,1, f);

	fclose(f);

	return imgdata;
}

bmpInfoHeader *createInfoHeader(unsigned w, unsigned h, unsigned ppp)
{
	bmpInfoHeader *ih = (bmpInfoHeader *)malloc(sizeof(bmpInfoHeader));

	ih->headersize=sizeof(bmpInfoHeader);
	ih->width=w;
	ih->height=h;
	ih->planes=1;
	ih->bpp=24;
	ih->compress=0;
	ih->imgsize=w*h*3;        /* 3 bytes por pixel w*h pixels */
	ih->bpmx=(unsigned)((double)ppp*100/2.54)/2;
	//ih->bpmx=(unsigned)round((double)ppp*100/2.54);
	ih->bpmy=ih->bpmx;        /* Misma resolución vertical y horiontal */
	ih->colors=0;
	ih->imxtcolors=0;

	return ih;
}

void SaveBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata)
{
	bmpFileHeader header;
	FILE *f;
	uint16_t type;

	f=fopen(filename, "w+");
	header.size=info->imgsize+sizeof(bmpFileHeader)+sizeof(bmpInfoHeader);
	/* header.resv1=0; */
	/* header.resv2=1; */
	/* El offset será el tamaño de las dos cabeceras + 2 (información de fichero)*/
	header.offset=sizeof(bmpFileHeader)+sizeof(bmpInfoHeader)+2;
	/* Escribimos la identificación del archivo */
	type=0x4D42;
	fwrite(&type, sizeof(type),1,f);
	/* Escribimos la cabecera de fichero */
	fwrite(&header, sizeof(bmpFileHeader),1,f);
	/* Escribimos la información básica de la imagen */
	fwrite(info, sizeof(bmpInfoHeader),1,f);
	/* Escribimos la imagen */
	fwrite(imgdata, info->imgsize, 1, f);
	fclose(f);
}

float CalcularNenC(unsigned char *vEntrada, int M,int s){
	float sPrima=(float)256/((float)M/(float)(s));
	float N=0;
	int listSize=M*M;
	int gridSize=s*s;
	int max=0,min=256;
	int inicio=0, pos=0;
	int nCol=M/s;
	for (int I = 0; I < M; I+=s)
	{
		for (int J = 0; J < M; J+=s)
		{
			//cada grid de S*S
			inicio=I*M+J;
			max=0;
			min=500000;
			//printf("\ni=%i, j=%i, pos=%i\n",I,J,inicio);
			for (int i = 0; i < s; i++)
			{
				for (int j = 0; j < s; j++)
				{
					pos=inicio+i*M+j;
					//printf("i=%i, j=%i, pos=%i valor=%i\n",i,j,pos,vEntrada[pos]);
					if(max<vEntrada[pos])max=vEntrada[pos];
					if(min>vEntrada[pos])min=vEntrada[pos];
					//if(min>pos)min=inicio;
				}
			}
			//printf("n(%f)=%i(max)/%i(min)\n",max/(float)sPrima-min/(float)sPrima+1,max,min);
			N+=max/(float)sPrima-min/(float)sPrima+1;
			//printf("\nmax=%i, min=%i, N=%f\n",max,min,N);
		}
	}
	//printf("Nde320=%f\n",N);
	return N;
}