/*********** Librerias usadas por el programa **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
/*******************************************************************************************************************************/
/************* Estructuras a utilizar ********************/


/************* Definiciones ********************/
#define CERO 0
//Codigos
#define TYPE		256
#define VAR			257
#define ARRAY		258
#define BEGIN		259
#define END			260
#define PR_DO		261
#define FUNCTION	262
#define PR_INTEGER	263
#define PR_REAL		264
#define PR_BOOLEAN	265
#define PR_CHAR		266
#define PR_FOR		267
#define PR_IF		268
#define PR_ELSE		269
#define PR_WHILE	270
#define PR_CASE		271
#define CONST		272
#define NUM			273
#define BOOL		274
#define NOT			275
#define OPREL		276
#define OPSUMA		277
#define OPMULT		278
#define OPASIGNA	279
// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101
/*******************************************************************************************************************************/
typedef struct entrada{
	//se define los campos de 1a entrada de la tabla de simbolos
	int compLex;
	char lexema[TAMLEX];	
	//al tener el valor null puede definir que la variable no este declarada
	struct entrada *tipoDato; 	
	
} entrada;
/*******************************************************************************************************************************/
typedef struct {
	int compLex;
	entrada *pe;
} token;

/************* Prototipos a tulizar ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void sigLex();


/*******************************************************************************************************************************/

