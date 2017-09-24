/*******************************************************************************************************************************/
/*********** Inclusión de cabecera **************/
#include "anlexico.h"
/*******************************************************************************************************************************/
/************* Variables globales **************/

int consumir;			/* 1 indica al analizador lexico que debe devolver
						el sgte componente lexico, 0 debe devolver el actual */

char cad[5*TAMANHLEXICO];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente pascal
char buff[2*TAMANHOBUFF];	// Buffer para lectura de archivo fuente
char id[TAMANHLEXICO];		// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/**************** Funciones **********************/


// Rutinas del analizador lexico
/*******************************************************************************************************************************/
void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}
/*******************************************************************************************************************************/
int main(int argc,char* args[])
{
	// inicializar analizador lexico

	initTabla();
	initTablaSimbolos();
	
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		while (t.compLex!=EOF){
			sigLex();
			printf("Lin %d: %s -> %d\n",numLinea,t.pe->lexema,t.compLex);
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
/*******************************************************************************************************************************/
void sigLex()
{
	int i = CERO;
	char c = CERO;
	int acepto = CERO;
	int estado = CERO;
	char mesg[50];
	entrada e;
	
	while((c=fgetc(archivo)!= EOF))
	{
		/********** ******************/
		//elimina los espacios en blanco
		if(c==' ' || c=='\t')
			continue;
		/********** ******************/
		//incremento en el numero de linea
		else if(c=='\n')
		{
			numLinea++;
			continue;
		}
		/********** ******************/
		//para identificar si es un identificador o una palabra reservada 
		else  if(isalpha(c))
		{
			i=CERO;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
				if(i>=TAMANHLEXICO)
					error("La Longitud del identificador supera el tamanho del buffer");
			}while(isalpha(c) || isdigit(c));
			id[i]='\0';
			if(c!=EOF)
				ungetc(c,archivo);
			else 
				c=CERO;
			t.pe=buscar(id);
			t.compLex=t.pe->compLex;
			if(t.pe->compLex == -1)
		    {
				strcpy(e.lexema,id);
				e.compLex=ident;
				insertar(e);
				t.pe=buscar(id);
				t.compLex=ident;
			}
			break;
		}
		/********** ******************/
		//para saber cual de los operadores relacional es
		else if(c=='>')
		{
		    c=fgetc(archivo);
            if(c=='='){
				t.compLex=OPRELACION;
				t.pe=buscar(">=");
			}else{
				ungetc(c,archivo);
				t.compLex=OPRELACION;
				t.pe=buscar(">");
			}
			break;
		}
		//para saber cual es la operacion basica a realizar
		/*********** **************************************/
		else if(c=='+')
		{
			t.compLex=OPBASICA;
			t.pe=buscar("+");
			break;
		}
		else if(c=='-')
		{
			t.compLex=OPBASICA;
			t.pe=buscar('-');
			break;
		}
		/*********** **************************************/
		//para saber cual operacion realizar division o multiplicacion
		/*********** **************************************/
		else if(c=='*')
		{
			t.compLex=OPMULTI;
			t.pe=buscar("*");
			break;
		}
		else if(c=='/')
		{
			t.compLex=OPMULTI;
			t.pe=buscar("*");
			break;
		}
		/*********** **************************************/
		//para la parte de relacion para los resultados
		/*********** **************************************/
		else if(c=='=')
		{
			t.compLex=OPRELACION;
			t.pe=buscar('=');
			break;
		
		/*********** **************************************/
		//separador para el array 
		else if(c==',')
		{
			t.compLex=',';
			t.pe=buscar(",");
			break;
		}
		/*********** **************************************/
		//para el termino de una linea de codigo	
		else if(c==';')
		{
			t.compLex=';';
			t.pe=buscar(";");
			break;
		}
		/*********** **************************************/
		//para poder realizar char de .
		else if(c=='.')
		{
			t.compLex='.';
			t.pe=buscar(".");
			break;
		}
		/*********** **************************************/
		//para la apertura de un array en json
		else if (c=='[')
		{
			t.compLex='[';
			t.pe=buscar("[");
			break;
		}
		/*********** **************************************/
		//para el cierre de un array en json
		else if (c==']')
		{
			t.compLex=']';
			t.pe=buscar("]");
			break;
		}
		/*********** **************************************/
		//si es un numero
		else if (isdigit(c))
		{
				i=0;
				estado=0;
				acepto=0;
				id[i]=c;
				/*********** **************************************/
				//una secuencia para saber si es . o e
				while(!acepto)
				{
					switch(estado){
					/*********** **************************************/
					case 0: 
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						//verifica si es . para un numero
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						//verifica si es e para un numero 
						else if(tolower(c)=='e'){
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					/*********** **************************************/
					//despues de un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
					case 1:
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(c=='.')
						{
							i--;
							fseek(archivo,-1,SEEK_CUR);
							estado=6;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					/*********** **************************************/
					//la parte decimal, pueden seguir con digitos o e
					case 2:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					/*********** **************************************/
					//una e, puede seguir +, - o digitos
					case 3:
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					/*********** **************************************/
					//necesariamente debe venir por lo menos un digito	
					case 4:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					/*********** **************************************/
					//una seguidilla de digitos correspondiente al exponente
					case 5:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}
						break;
					/*********** **************************************/
					//estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
					case 6:
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;
						id[++i]='\0';
						acepto=1;
						t.pe=buscar(id);
						if (t.pe->compLex==-1)
						{
							strcpy(e.lexema,id);
							e.compLex=NUM;
							insertar(e);
							t.pe=buscar(id);
						}
						t.compLex=NUM;
						break;
					/*********** **************************************/
					//el fin de archivo si llego
					case -1:
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else
							error(msg);
						exit(1);
					}
				}
			break;
		}
		///MIRAR ACA 
		/*********** **************************************/
		//para inicio de una operacion
		else if (c=='(')
		{
			/*********** **************************************/
			//es para ver si hay alguna concatenacion
			if ((c=fgetc(archivo))=='*')
			{
				while(c!=EOF)
				{
					c=fgetc(archivo);
					if (c=='*')
					{
						if ((c=fgetc(archivo))==')')
						{
							break;
						}
					}
					//aumenta el numero de linea
					else if(c=='\n')
					{
						numLinea++;
					}
				} 
				/*********** **************************************/
				//se llego al fin del archivo
				if (c==EOF)
					error("Se llego al fin de archivo sin finalizar un comentario");
				continue;
			}
			/*********** **************************************/
			
			else
			{
				ungetc(c,archivo);
				t.compLex='(';
				t.pe=buscar("(");
			}
			break;
		}
				else if (c==')')
		{
			t.compLex=')';
			t.pe=buscar(")");
			break;
		}

		//si es un caracter o una cadena de caracteres
		else if (c=='\'')
		{
			i=0;
			id[i]=c;
			i++;
			do{
				c=fgetc(archivo);
				if (c=='\'')
				{
					c=fgetc(archivo);
					if (c=='\'')
					{
						id[i]=c;
						i++;
						id[i]=c;
						i++;
					}
					else
					{
						id[i]='\'';
						i++;
						break;
					}
				}
				else if(c==EOF)
				{
					error("Se llego al fin de archivo sin finalizar un literal");
				}
				else{
					id[i]=c;
					i++;
				}
			}while(isascii(c));
			id[i]='\0';
			if (c!=EOF)
				ungetc(c,archivo);
			else
				c=0;
			t.pe=buscar(id);
			t.compLex=t.pe->compLex;
			if (t.pe->compLex==-1)
			{
				strcpy(e.lexema,id);
				if (strlen(id)==3 || strcmp(id,"''''")==0)
					e.compLex=CAR;
				else
					e.compLex=LITERAL;
				insertar(e);
				t.pe=buscar(id);
				t.compLex=e.compLex;
			}
			break;
		}
		/********** ******************/
		//para el incio de un array, programa o funcion
		else if (c=='{')
		{
			//se elimina el comentario
			while(c!=EOF)
			{
				c=fgetc(archivo);
				if (c=='}')
					break;
				//aumenta el numero de lineas
				else if(c=='\n')
				{
					numLinea++;
				}
			}
			//se llevo al final del  archivo
			if (c==EOF)
				error("Se llego al fin de archivo sin finalizar un comentario");
		}
		else if (c!=EOF)
		{
			sprintf(msg,"%c no es esperado",c);
			error(msg);
		}
		/********** ******************/
	}
	/********** ******************/
	//Fin del archivo
	if (c==EOF)
	{
		t.compLex=EOF;
		sprintf(e.lexema,"EOF");
		t.pe=&e;
	}
	/********** ******************/
}

