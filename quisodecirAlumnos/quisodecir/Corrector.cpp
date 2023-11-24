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
#define MAXLEIDAS 69000
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
void CambiaCadenas(char cad1[], char cad2[])
{
	//funcion para cambiar las cadenas
	char tempo[TAMTOKEN] = "/0";
	strcpy_s(tempo, TAMTOKEN, cad1);
	strcpy_s(cad1, TAMTOKEN, cad2);
	strcpy_s(cad2, TAMTOKEN, tempo);
}
/* Algo esta mal con la funcion 
void OrdenaPalabras(char szPalabras[][TAMTOKEN], int numElementos, int iEstadisticas[])
{
	for (int i = 0; i < numElementos - 1; i++)
	{
		for (int j = i + 1; j < numElementos -1; j++)
		{
			if (strcmp(szPalabras[i], szPalabras[j]) > 0)
			{
				char temp[TAMTOKEN];
				strcpy_s(temp, szPalabras[i]);
				strcpy_s(szPalabras[i], szPalabras[j]);
				strcpy_s(szPalabras[j], temp);

				int tempEstadistica = iEstadisticas[i];
				iEstadisticas[i] = iEstadisticas[j];
				iEstadisticas[j] = tempEstadistica;
			}
		}
	}
}*/
void eliminarRepetidas(char szPalabras[][TAMTOKEN], int& numElementos, int iEstadisticas[]) 
{
	int NoRepetido = 0; 
	// �ndice para palabras no repetidas

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
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	//variables creadas
	char linea[4000];
	char PalabraDet[TAMTOKEN] = "\0";
	int  indicePD = 0;
	iNumElementos = 0;
	int inumpalabras = 0;
	int palabrasleidas = 0;
	int i;
	//Abrir el archivo
	FILE* fpdicc;
	fopen_s(&fpdicc, szNombre, "r");
	if (fpdicc != NULL)
	{
		if (DEPURAR == 1)
		{
			printf("\nSi lo pude abrir\n");
		}

		//empezar lectura del archivo 
		// Lee una l�nea

		while (fgets(linea, sizeof(linea), fpdicc) != NULL) 
		{
			pintatexto(linea);

			// Convertir toda la l�nea a min�sculas
			for ( i = 0; linea[i] != '\0'; i++)
			{
				linea[i] = tolower(linea[i]);
			}
			// Eliminar espacios en blanco al principio y al final de la l�nea
			i = 0;
			while (linea[i] == ' ' || linea[i] == '\t') 
			{
				i++;
			}

			int j = strlen(linea) - 1;
			while (linea[j] == ' ' || linea[j] == '\t')
			{
				linea[j] = '\0';
				j--;
			}
			// Procesar cada car�cter de la l�nea
			for (int i = 0; linea[i] != '\0'; i++) {
				char caracter = linea[i];

				// Verifica si el car�cter es un delimitador de palabra
				if (caracter == ' ' || caracter == '\t' || caracter == '\n' || caracter == ',' || caracter == ';' || caracter == '(' || caracter == ')' || caracter == '.' || caracter == '\r') {
					// Verifica si la palabra actual no est� vac�a
					if (indicePD > 0) {
						// Termina la palabra actual y la guarda en el diccionario
						PalabraDet[indicePD] = '\0';

						// Verificar si la palabra ya existe en el arreglo
						int indiceExistente = -1;
						int existe = 1;
						for (int j = 0; j < iNumElementos && existe == 1; j++) {
							if (strcmp(szPalabras[j], PalabraDet) == 0) {
								indiceExistente = j;
								existe = 0;
							}
						}

						if (indiceExistente != -1) {
							// La palabra ya existe, actualizar estad�sticas
							iEstadisticas[indiceExistente]++;
						}
						else {
							// La palabra no existe, agregar al arreglo
							if (strcmp(PalabraDet, ",") != 0 && strcmp(PalabraDet, ".") != 0 && strcmp(PalabraDet, "(") != 0 && strcmp(PalabraDet, ")") != 0 && strcmp(PalabraDet, ";") != 0) {
								strcpy_s(szPalabras[iNumElementos], TAMTOKEN, PalabraDet);
								iEstadisticas[iNumElementos] = 1;
								iNumElementos++;
							}
						}

						// Reiniciar la palabra actual
						indicePD = 0;
					}
				}
				else {
					// Agregar el car�cter a la palabra actual si no es un delimitador
					if (indicePD < TAMTOKEN - 1) {
						if (caracter != ',' && caracter != '.' && caracter != '(' && caracter != ')' && caracter != ';' && caracter != '.') {
							PalabraDet[indicePD++] = caracter;
						}
						else if (caracter == '.') {
							// Si es un punto, guarda la palabra actual y reinicia para el pr�ximo token
							if (indicePD > 0) {
								PalabraDet[indicePD] = '\0';

								int indiceExistente = -1;
								int existe = 1;
								for (int j = 0; j < iNumElementos && existe == 1; j++) {
									if (strcmp(szPalabras[j], PalabraDet) == 0) {
										indiceExistente = j;
										existe = 0;
									}
								}

								if (indiceExistente != -1) {
									iEstadisticas[indiceExistente]++;
								}
								else {
									if (strcmp(PalabraDet, ",") != 0 && strcmp(PalabraDet, ".") != 0 && strcmp(PalabraDet, "(") != 0 && strcmp(PalabraDet, ")") != 0 && strcmp(PalabraDet, ";") != 0) {
										strcpy_s(szPalabras[iNumElementos], TAMTOKEN, PalabraDet);
										iEstadisticas[iNumElementos] = 1;
										iNumElementos++;
									}
								}

								indicePD = 0;
							}
						}
					}
				}
			}
		}
			//revisar que no falte una palabra
			if (indicePD > 0 && strlen(PalabraDet) > 0)
			{
				PalabraDet[indicePD] = '\0';
				pintatexto(PalabraDet);
				iNumElementos++;
			}
			//primero se ordenan para luego quitar las repeticiones, si aun queda repeticion
			OrdenaPalabras(szPalabras, iNumElementos, iEstadisticas);
			//eliminarRepetidas(szPalabras, iNumElementos, iEstadisticas);
			pintanum(iNumElementos);
		

		//cerrar el archivo
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
// elimina las palabras que se repitan entre los arreglos
	int i, j;

	iNumLista = 0;
	for (i = 0; i < iNumSugeridas; i++)
	{
		for (j = 0; j < iNumElementos; j++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0)
			{
				strcpy_s(szListaFinal[iNumLista], TAMTOKEN ,szPalabras[j]);
				iPeso[iNumLista++] = iEstadisticas[j];
			}
		}
	}


	//Acomoda de mayor a menor
	int tempo1, tempo2;
	char tempoC1[TAMTOKEN], tempoC2[TAMTOKEN];
	
	//reinicio de variables 
	i = 0;
	j = 0;
	while (iNumLista > i)
	{
		for (j = i + 1; j < iNumLista; j++ )
		{
			if (iPeso[i] < iPeso[j])
			{
				//metodo para intercambiar los pesos
				tempo1 = iPeso[i];
				tempo2 = iPeso[j];
				iPeso[i] = tempo2;
				iPeso[j] = tempo1;

				//intercambiar las cadenas 
				
				//copiar las palabras de la lista al espacio temporal
				strcpy_s(tempoC1,TAMTOKEN, szListaFinal[i]);
				strcpy_s(tempoC2,TAMTOKEN, szListaFinal[j]);

				//regresar las palabras a la lista final
				strcpy_s(szListaFinal[i],TAMTOKEN, tempoC2);
				strcpy_s(szListaFinal[j],TAMTOKEN, tempoC1);
			}
		}
		i++;
	}
	//Quita repetidos.
	int numsugeridas = iNumLista;
	

	for (i = 0; i < numsugeridas; i++)
	{
		for (j = i + 1; j < numsugeridas;)
		{
			if (strcmp(szListaFinal[i], szListaFinal[j]) == 0) 
			{
				// Si se encuentra un duplicado, mover el �ltimo elemento al lugar del duplicado y reducir numsugeridas
				strcpy_s(szListaFinal[j], TAMTOKEN, szListaFinal[numsugeridas - 1]);
				iPeso[j] = iPeso[numsugeridas - 1];
				numsugeridas--;
			}
			else {
				j++; // Solo incrementa j si no se elimino elemento
			}
		}
	}
	iNumLista = numsugeridas;
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
	int i, j;
	// Captura la palabra para modificarla
	char szPalabramod[TAMTOKEN];
	strcpy_s(szPalabramod, TAMTOKEN, szPalabraLeida);

	int longitud = strlen(szPalabramod);
	int numCandidatas = 0;

	// Letras del alfabeto,
	char letras[] = "abcdefghijklmnopqrstuvwxyz�����";

	// A�ade la palabra original
	strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, szPalabramod);
	numCandidatas++;

	// Elimina un caracter
	for (i = 0; i < longitud; i++)
	{
		char palabraGenerada[TAMTOKEN];
		int indice = 0;

		for (j = 0; j < longitud; j++)
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
	for (i = 0; i < longitud - 1; i++) {
		char palabraGenerada[TAMTOKEN];
		strcpy_s(palabraGenerada, TAMTOKEN, szPalabramod);

		char temp = palabraGenerada[i];
		palabraGenerada[i] = palabraGenerada[i + 1];
		palabraGenerada[i + 1] = temp;

		strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGenerada);
		numCandidatas++;
	}

	// Cambia las letras usando el arreglo de letras
	for (i = 0; i < longitud; i++) {
		for ( j = 0; j < sizeof(letras) - 1; j++) {
			char palabraGenerada[TAMTOKEN];
			strcpy_s(palabraGenerada, TAMTOKEN, szPalabramod);
			palabraGenerada[i] = letras[j];

			strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, palabraGenerada);
			numCandidatas++;
		}
	}
	// Inserta el alfabeto entre cada caracter de la palabra
	for (i = 0; i < longitud; i++) {
		for (j = 0; j < sizeof(letras) - 1; j++) {
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
		
	}
	// Inserta al final usando el arreglo de letras
	for (j = 0; j < sizeof(letras) - 1; j++) {
		szPalabramod[longitud] = letras[j];
		szPalabramod[longitud + 1] = '\0';

		strcpy_s(szPalabrasSugeridas[numCandidatas], TAMTOKEN, szPalabramod);
		numCandidatas++;
	}

	iNumSugeridas = numCandidatas;
}