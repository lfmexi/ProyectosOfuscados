#ifndef FUNCIONESESTRUCTURAS_H_INCLUDED
#define FUNCIONESESTRUCTURAS_H_INCLUDED
int addColumna(Matriz**modelo,double valor);

int addFila(Matriz**modelo,double valor);

int get_size_lista(Lista*l);

int matriz_set_costo(Matriz**modelo,int x,int y,double costo);

int matriz_set_asignado(Matriz**modelo,int x,int y,double asignado);

int lista_add(Lista**l,double valor);

NodoLista*lista_getNodo(Lista*l,int i);

double lista_get_sum(Lista*l);

void imprimirLista(Lista*l);

void imprimirCostos(Matriz*modelo);

void imprimirMatriz(Matriz*modelo);
#endif // FUNCIONESESTRUCTURAS_H_INCLUDED
