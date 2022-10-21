#ifndef LISTA_H
#define LISTA_H

#include "juego.h"
#include "server.h"

struct lista
{
 user usuario;
 struct lista *sig;
};

struct lista *nuevoElemento();
void imprimirLista(struct lista *cabeza);
int buscarElemento(struct lista *cabeza, int n);
void insertarDetras(struct lista **cabeza, int n);
void borrarElemento(struct lista **cabeza, int n);
void borrarLista(struct lista **elemento);
void insertarOrden(struct lista **cabeza, int n);

#endif