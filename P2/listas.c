#include <stdio.h>
#include <malloc.h>

#include "listas.h"


struct lista *nuevoElemento()
{
 return ((struct lista *)malloc(sizeof(struct lista)));
}

void imprimirLista(struct lista *cabeza)
{
 struct lista *aux = NULL;
 
 aux = cabeza;
 while (aux != NULL)
  {
   printf(" %d \n", aux->n);
   aux = aux->sig;
  }
}

int buscarElemento(struct lista *cabeza, int n)
{
 int encontrado = 0;
 struct lista *aux = NULL;
 
 aux = cabeza;
 /* Se recorre la lista hasta encontrar el elemento o hasta llegar al final */
 while (aux != NULL && encontrado ==0)
  {
   if (aux->n == n)
    encontrado = 1;
   else
    aux = aux->sig;
  }
 return encontrado;
}


void insertarDetras(struct lista **cabeza, int n)
{
 struct lista *nuevo = NULL;
 struct lista *aux = NULL;

 /* se reserva espacio para el nuevo elemento */
 nuevo = nuevoElemento();
 nuevo->n = n;
 nuevo->sig = NULL;

 if (*cabeza == NULL) /* la lista est� vacia y el nuevo ser� la cabeza */
  *cabeza = nuevo;
 else /* se localiza el �ltimo elemento para enlazarlo al nuevo */
  {
   aux = *cabeza;
   while(aux->sig != NULL)
   {
    aux = aux->sig;
   }
   aux->sig = nuevo;
  }  
}


void borrarElemento(struct lista **cabeza, int n)
{
 struct lista *ant = NULL; /* almacena el elemento anterior al que se borra*/ 
 struct lista *aux = NULL; /* almacena el elemento a borrar */
 
 aux = *cabeza;
 /* Busqueda del elemento a borrar y su anterior */
 while (aux->n != n)
  {
   ant = aux;
   aux = aux->sig;
  }
  
 if (aux == *cabeza) /* el elemento a borrar es la cabeza */
  {
   *cabeza = aux->sig; /* la nueva cabeza es el siguiente */
   free(aux); /* se libera la antigua cabeza */
  }
 else /* El elemento a borrar no es la cabeza */
  {
   ant->sig = aux->sig; /* Se enlaza el anterior con el siguiente */
   free(aux); /* se libera el nodo a borrar */
  }
}

void borrarLista(struct lista **cabeza)
{
   struct lista * aux, * sig;
   aux=*cabeza;
   while(aux!=NULL)
   {
     sig=aux->sig;
     free(aux);
     aux=sig;
   }
   *cabeza=NULL;
}


        

