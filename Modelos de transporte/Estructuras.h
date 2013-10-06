#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct casilla{
    double costo;
    double asignado;
    double costo_marginal;
    int utilizable;
    char signo;
    int optimo;
}Casilla;

typedef struct nodo{
    struct nodo* arriba;
    struct nodo* abajo;
    struct nodo* izquierda;
    struct nodo* derecha;
    int x;
    int y;
    Casilla* contenido;
}NodoOrtogonal;

typedef struct nodoLista{
    struct nodoLista* sig;
    struct nodoLista* ant;
    NodoOrtogonal* ort;
    double valor;
    double coef;
    int utilizable;
}NodoLista;

typedef struct lista{
    NodoLista* inicio;
    NodoLista* fin;
}Lista;

typedef struct matriz{
    Lista* filas;
    Lista* columnas;
}Matriz;

typedef struct punto{
    int x;
    int y;
}Punto;

typedef struct penalizacion{
    double p;
    int pos;
}Penalizacion;
#endif // ESTRUCTURAS_H_INCLUDED
