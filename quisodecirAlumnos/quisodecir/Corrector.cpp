/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Alvarez Cortes Aureliano 321118590
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include <ctype.h>
#define DEPURAR 0
//Funciones para pintar 
void pintanum(int numero)
{
	if (DEPURAR == 1)
	{
		printf("%i\n", numero);
	}
}
void pintatexto(char cadena[])
{
	if (DEPURAR == 1)
		printf("%s\n", cadena);
}
void pintatexto2(char cadena[])
{
	if (DEPURAR == 1)
		printf("%s\n", cadena);
}
void OrdenaPalabras(char szPalabras[][TAMTOKEN], int numElementos) 
{
	int i, j, nocambios = 1;
	for (i = 0; i < numElementos - 1 && nocambios == 1; i++)
	{
		nocambios = 0;
		for (j = 0; j < numElementos - i - 1; j++) 
		{
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				// intercambio de cadenas 
				char tempo[TAMTOKEN];
				strcpy_s(tempo, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, tempo);
				nocambios = 1;
			}
		}
	}
}

void eliminarRepetidas(char szPalabras[][TAMTOKEN], int& numElementos, int iEstadisticas[]) 
{
	int NoRepetido = 0; 
	// Índice para palabras no repetidas

	for (int i = 0; i < numElementos; i++) {
		int j = 0, esRepetida = 0;

		// revisa si se repite
		while (j < NoRepetido && !esRepetida)
		{
			if (strcmp(szPalabras[i], szPalabras[j]) == 0)
			{
				// si se repite incrementa
				iEstadisticas[j]++;
				esRepetida = 1;
			}
			j++;
		}

		if (!esRepetida) 
		{
			// La palabra no es repetida, y la cambia 
			strcpy_s(szPalabras[NoRepetido], TAMTOKEN, szPalabras[i]);
			iEstadisticas[NoRepetido] = iEstadisticas[i];
			NoRepetido++;
		}
	}
	//cambia los elementos no repetidos 
	numElementos = NoRepetido;
}

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario		(
	char* szNombre, 
	char szPalabras[][TAMTOKEN], 
	int iEstadisticas[], 
	int& iNumElementos	)
{
	//variables creadas
	char linea[NUMPALABRAS];
	char PalabraDet[TAMTOKEN] = "\0";
	int i, indicePD = 0, longitudlinea, longiPD;
	iNumElementos = 0;
	//Abrir el archivo
	FILE* fpdicc;
		if (DEPURAR == 1)
		{
			printf("%s", szNombre);
		}
	fopen_s(&fpdicc, szNombre, "r");
	//Preguntar si el archivo se abrio
	if (fpdicc != NULL)
	{
		if (DEPURAR == 1)
		{
			printf("\nSi lo pude abrir\n");
		}
		
		//empezar lectura del archivo 
		while (fgets(linea, sizeof(linea), fpdicc) != NULL)
		{
			//Lee
			pintatexto(linea);
			
			//separar las palabras
			longitudlinea = strlen(linea);

			for (i = 0; i <= longitudlinea && longitudlinea < NUMPALABRAS; i++) 
			{
				//detectar una palabra
				if (linea[i] == ' ' || linea[i] == '\0' || linea[i] == '\n') 
				{ 
					PalabraDet[indicePD] = '\0';
					//una vez detectada para guardar en el diccionario
					strcpy_s(szPalabras[iNumElementos], TAMTOKEN ,PalabraDet);
					iEstadisticas[iNumElementos] = 1;
					longiPD = strlen(PalabraDet);
					if (longiPD > 0)
					{ 
						// verifica si la palabra no esta vacía
						pintatexto(PalabraDet);
						iNumElementos++;
						indicePD = 0;
					}
				}
				else
				{
					if (linea[i] != ',' && linea[i] != '.' && linea[i] != '(' && linea[i] != ')' && linea[i] != '\t')
					{   //guarda el caracter en minuscula
						PalabraDet[indicePD] = tolower(linea[i]);
						indicePD++;
					}
				}
				
			}
			//revisar que no falte una palabra
			if (indicePD > 0 && longiPD > 0)
			{
				PalabraDet[indicePD] = '\0';
				pintatexto(PalabraDet);
				iNumElementos++;
			}
			
		}
		//primero se ordenan para luego quitar las repeticiones 
		OrdenaPalabras(szPalabras, iNumElementos);
		eliminarRepetidas(szPalabras, iNumElementos, iEstadisticas);
		pintanum(iNumElementos);

		//Cerrar el archivo
		fclose(fpdicc);
	}
	else
	{
		if (DEPURAR == 1)
		{
			printf("\nNo lo pude abrir\n");
		}
	}




}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista			)					//Numero de elementos en la szListaFinal
{
	


	//Sustituya estas lineas por su codigo


	strcpy_s(szListaFinal[0],TAMTOKEN, szPalabrasSugeridas[0]); //la palabra candidata
	iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata

	iNumLista = 1;							//Una sola palabra candidata
}


/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void ClonaPalabras(
	char* szPalabraLeida,
	char szPalabrasSugeridas[][TAMTOKEN],
	int& iNumSugeridas)
{
	// Captura la palabra para modificarla
	char szPalabramod[TAMTOKEN];
	strcpy_s(szPalabramod, TAMTOKEN, szPalabraLeida);

	int longitud = strlen(szPalabramod);
	int numCandidatas = 0;

	// Letras del alfabeto,
	char letras[] = "abcdefghijklmnopqrstuvwxyzáéíóú";

	// Añade la palabra original
	strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, szPalabramod);
	numCandidatas++;

	// Elimina un caracter
	for (int i = 0; i < longitud; i++)
	{
		char palabraGenerada[TAMTOKEN];
		int indice = 0;

		for (int j = 0; j < longitud; j++)
		{
			if (j != i)
			{
				palabraGenerada[indice] = szPalabramod[j];
				indice++;
			}
		}

		palabraGenerada[indice] = '\0';
		strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGenerada);
		numCandidatas++;
	}

	// Intercambia caracteres
	for (int i = 0; i < longitud - 1; i++) {
		char palabraGenerada[TAMTOKEN];
		strcpy_s(palabraGenerada, TAMTOKEN, szPalabramod);

		char temp = palabraGenerada[i];
		palabraGenerada[i] = palabraGenerada[i + 1];
		palabraGenerada[i + 1] = temp;

		strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGenerada);
		numCandidatas++;
	}

	// Cambia las letras usando el arreglo de letras
	for (int i = 0; i < longitud; i++) {
		for (int j = 0; j < sizeof(letras) - 1; j++) {
			char palabraGenerada[TAMTOKEN];
			strcpy_s(palabraGenerada, TAMTOKEN, szPalabramod);
			palabraGenerada[i] = letras[j];

			strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGenerada);
			numCandidatas++;
		}
	}
	// Inserta el alfabeto entre cada caracter de la palabra
	for (int i = 0; i < longitud; i++) {
		for (int j = 0; j < sizeof(letras) - 1; j++) {
			//Una palabra nueva para recorrer el nulo 
			char palabraGen[TAMTOKEN];
			int indice = 0;

			for (int k = 0; k < longitud; k++) {
				if (k == i) {
					palabraGen[indice++] = letras[j];
				}
				palabraGen[indice++] = szPalabramod[k];
			}

			palabraGen[indice] = '\0';
			strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGen);
			numCandidatas++;
		}
		// Inserta al final usando el arreglo de letras
		for (int j = 0; j < sizeof(letras) - 1; j++) {
			szPalabramod[longitud] = letras[j];
			szPalabramod[longitud + 1] = '\0';

			strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, szPalabramod);
			numCandidatas++;
		}

		iNumSugeridas = numCandidatas;
	}
}