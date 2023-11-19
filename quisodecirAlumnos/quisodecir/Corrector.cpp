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
//funcion para arreglar las cadenas
void CambiaCadenas(char cad1[], char cad2[])
{
	//funcion para cambiar las cadenas
	char tempo[TAMTOKEN];
	strcpy_s(tempo, TAMTOKEN, cad1);
	strcpy_s(cad1, TAMTOKEN, cad2);
	strcpy_s(cad2, TAMTOKEN, tempo);
}
void OrdenaPalabras(char szPalabras[][TAMTOKEN], int numElementos) 
{
	//aplica metodo de burbuja
	int i, j;
	for (i = 0; i < numElementos - 1; i++)
	{
		for (j = 0; j < numElementos - i - 1; j++)
		{
			//compara las cadenas para ver si debe haber un cambio 
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				CambiaCadenas(szPalabras[j], szPalabras[j + 1]);
			}
		}
	}
}
void eliminarRepetidas(char szPalabras[][TAMTOKEN], int& numElementos, int iEstadisticas[]) {
	for (int i = 0; i < numElementos - 1; i++)
	{
		for (int j = i + 1; j < numElementos;) 
		{
			//revisar si la que sigue es igual
			if (strcmp(szPalabras[i], szPalabras[j]) == 0) 
			{
				// palabra repetida
				for (int k = j; k < numElementos - 1; k++) 
				{
					//copia la palabra que sigue para quitar repeticion
					strcpy(szPalabras[k], szPalabras[k + 1]);
					// actualizar estadísticas
					iEstadisticas[k] = iEstadisticas[k + 1];
				}
				//disminuye la cantidad de arreglos
				numElementos--;
				// incrementar la estadística
				iEstadisticas[i]++; 
			}
			else {
				j++;
			}
		}
	}
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
	int i, indicePD = 0;
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
		while (!feof(fpdicc))
		{
			//Lee
			fgets(linea, sizeof(linea), fpdicc);
			pintatexto(linea);
			
			//separar las palabras
			for (i = 0; i <= strlen(linea); i++)
			{
				//detectar una palabra
				if (linea[i] == ' ' || linea[i] == '\0' || linea[i] == '\n') 
				{ 
					PalabraDet[indicePD] = '\0';
					//una vez detectada para guardar en el diccionario
					strcpy_s(szPalabras[iNumElementos], TAMTOKEN ,PalabraDet);
					iEstadisticas[iNumElementos] = 1;
					if (strlen(PalabraDet) > 0)
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
			if (indicePD > 0 && strlen(PalabraDet) > 0)
			{
				PalabraDet[indicePD] = '\0';
				pintatexto(PalabraDet);
				iNumElementos++;
			}
			//primero se ordenan para luego quitar las repeticiones 
			OrdenaPalabras(szPalabras, iNumElementos);
			eliminarRepetidas(szPalabras, iNumElementos,iEstadisticas);
			pintanum(iNumElementos);
		}

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
void	ListaCandidatas(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista)							//Numero de elementos en la szListaFinal
{

	//Sustituya estas lineas por su codigo
	strcpy(szListaFinal[0], szPalabrasSugeridas[0]); //la palabra candidata
	iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata

	iNumLista = 1;							//Una sola palabra candidata
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
{
	//Sustituya estas lineas por su codigo
	strcpy(szPalabrasSugeridas[0], szPalabraLeida); //lo que sea que se capture, es sugerencia
	iNumSugeridas = 1;							//Una sola palabra sugerida
}
