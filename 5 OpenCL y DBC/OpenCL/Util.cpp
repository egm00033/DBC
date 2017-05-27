#include "stdafx.h"
#include "Util.h"


/*
*leerImagen: carga en memoria una matriz con la información de la imagen
*rellena el ancho y alto de la imagen
*
*/

int **leerImagenBMP(char *nome,long &ancho,long &alto){
	FILE *fpin;
	char   id[] = "  ";
	long int despl, bxp;
	int **matriz;


	if ((fpin = fopen (nome,"rb")) == NULL) {
		fprintf (stderr, "Error al abrir el archivo %s\n", nome);
		exit (EXIT_FAILURE);
	}
	//extraccion de metadatos
	printf ("Datos de la imagen:\n");
	fread (id, 2, 1, fpin);
	printf ("\tIdentificador de bitmap: %s\n", id);
	fseek (fpin, 10L, SEEK_SET);
	fread (&despl, sizeof (long), 1, fpin);
	printf ("\tDesplazamiento a los datos: %ld\n", despl);
	fseek (fpin, 18L, SEEK_SET);
	fread (&ancho, sizeof (long), 1, fpin);
	fread (&alto, sizeof (long), 1, fpin);
	printf ("\tDimensiones de la imagen, ancho: %ld, alto: %ld\n", ancho, alto);
	fseek (fpin, 28L, SEEK_SET);
	fread (&bxp, sizeof (long), 1, fpin);
	printf ("\tBits por pixel: %ld\n", bxp);
	//comprobar que es bmp
	if (strcmp (id, "BM")){
		fprintf (stderr, "La imagen debe ser un bitmap.\n");

	}else {
		int i;
		FILE *fpout;
		fseek (fpin, 0L, SEEK_SET);
		fpout = fopen (".\\images\\saida.bmp", "wb");
		for (i = 0; i < despl; i++)
			fputc (fgetc (fpin), fpout);

		//pasar a 8bxp
		char bpp=8;
		//el tamaño es multiplo de 4 y se rellena con 0
		//se elimina el relleno
		int desplazamiento=4-(ancho)%4;
		if(desplazamiento==4)desplazamiento=0;
		ancho+=desplazamiento;
		//almacenar en un vector todos los pixels
		char *vector=new char [ancho*alto];
		printf("tamaño %d\n",ancho*alto);
		for (i = 0; i < (ancho) * (alto); i++) {
			char red;
			fread (&red, sizeof (char), 1, fpin);
			vector[i]=red;
			fwrite (&red, sizeof (char), 1, fpout);

		}
		printf(" \n");

		//limpiar vector y crear matriz ordenada


		int anchoMatriz=ancho-desplazamiento;
		int altoMatriz=alto;
		//reserva de memoria para la matriz
		matriz =(int **) calloc(altoMatriz,sizeof(int *));
		for (int i = 0; i < altoMatriz; i++){
			matriz[i]=(int *) calloc(anchoMatriz,sizeof(int));
		}

		int fila=0,col=0;

		for (i = 0; i < (ancho) * (alto); i++) {

			if(ancho-(i)%(ancho)<=desplazamiento){
				//borrar relleno
			}else{
				if(col==altoMatriz){
					col=0;
					fila+=1;
					//salto de linea
				}
				matriz[fila][col]=(uint8_t)vector[i] & 0xff;//leer en unsigned int de 1Byte
				if(matriz[fila][col]<0){
					printf("%i \n",matriz[fila][col]);//comprobar que no hay pixels invalidos
				}
				col+=1;
			}

		}
		ancho-=desplazamiento;//eliminar exceso
		printf("\n"); 
		printf("\n"); 
		//mostrar


		printf ("tam char : %i byte\n", sizeof (char));

		fclose (fpout);

		free(vector);
	}
	fclose (fpin);
	return matriz;
}


