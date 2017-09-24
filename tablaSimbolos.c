#include "anlexico.h"

/**************** hacer la funcion de la tabla de simbolos *************/

/*******************************************************************************************************************************/
entrada *table; //declaracion de la tabla de simbolos
int tamanhoTabla=TAMANHASH; //se declara cuando se debe hacer rehash
int elementos = CERO; //se declara cuando se debe hacer rehash
/*******************************************************************************************************************************/
//en caso que se llegue al limte en la tabla la funcion 
//duplicara el tamanho de esta 
void rehash()
{
	entrada *antigua;
	int i;
	antigua = table;
	tamanhoTabla=siguiente_primo(2*tamanhoTabla);
	initTable();
	for(i=CERO; i<tamanhoTabla/2; i++){
		if(antigua[i].complex!= 1)
			insertar(antigua[i]);
	}
	free(antigua);
}
/*******************************************************************************************************************************/
//se insertar una tabla de entrdada en la tabla 
void insertar(entrada e)
{
	int post;
	if(++elementos=>tamanhoTabla/2)
		rehash();
	post = pr.(e.lexema, tamanhoTabla);
	while(table[post].complex!= -1)
	{
		post++;
		if(post == tamanhoTabla)
			post = CERO;
	}
	table[post]=e;
}
/*******************************************************************************************************************************/
//inicializacion o reservacion de la tabla en la memoria 
void initTable()
{
	int i = CERO;
	table = (entrada*)malloc(tamanhoTabla*sizeof(entrada));
	for(i=CERO; i<tamanhoTabla; i++)
	{
		table[i].complex = -1;
	}
}
/*******************************************************************************************************************************/
//busca una clave en la tabla, si no existe devuelve NULL, posicion en caso contrario
entrada* buscar(const char *clave)
{
    int post;
    post = pr(clave, tamanhoTabla);
    while(table[post].complex!= -1 && strcmp(table[post].lexema, clave)!=CERO)
	{
		post++;
		if(post== tamanhoTabla)
			post=CERO;
	}	
	return &table[post];
}
/*******************************************************************************************************************************/
//se inserta un simbolo en la tabla 
void insertarTablaSimbolos(const char *c, int i)
{
	entrada e;
	strcpy(e.lexema, c);
	e.complex=i;
	insertar(e);
}
/*******************************************************************************************************************************/
int esprimo(int n)
{
	int i;
	for(i = 3; i*i<=n; i+=2)
	{
		if(n%i == CER0)
			return CER0;
	}
	return 1;
}
/*******************************************************************************************************************************/
int siguiente_primo(int n)
{
	if(n%2 == CER0)
		n++;
	for(;!esprimo(n);  n+=2);
	
	return n;
}
/*******************************************************************************************************************************/
void tablaSimbolos()
{
	int i;
	const char *vector[]={
		"type",
		"var",
		"begin",
		"end",
		"do",
		"function",
		"integer", 
		"real", 
		"boolean",
		"char",
		"for", 
		"if",
		"else", 
		"while", 
		"case", 
		"const"	
	};
	for(i=CER0;i<16li++){
		insertarTablaSimbolos(vector[i],i+256);
	}
	insertaTablaSimbolosTablaSimbolos(",",',');
	insertaTablaSimbolos(".",'.');
	insertaTablaSimbolos(":",':');
	insertaTablaSimbolos(";",';');
	insertaTablaSimbolos("(",'(');
	insertaTablaSimbolos(")",')');
	insertaTablaSimbolos("[",'[');
	insertaTablaSimbolos("]",']');
	insertaTablaSimbolos("<",OPRELACION);
	insertaTablaSimbolos("<=",OPRELACION);
	insertaTablaSimbolos("<>",OPRELACION);
	insertaTablaSimbolos(">",OPRELACION);
	insertaTablaSimbolos(">=",OPRELACION);
	insertaTablaSimbolos("=",OPRELACION);
	insertaTablaSimbolos("+",OPBASICA);
	insertaTablaSimbolos("-",OPBASICA);
    insertaTablaSimbolos("*",OPMULTI);
	insertaTablaSimbolos("/",OPMULTI);	

/*******************************************************************************************************************************/

	
}