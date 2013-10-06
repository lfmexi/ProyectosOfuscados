#ifndef FUNCIONESMODELOS_H_INCLUDED
#define FUNCIONESMODELOS_H_INCLUDED
int agrega(Matriz**modelo);

Matriz* esquina_noroeste(Matriz*modelo);

Punto* get_pos_minimo(Matriz*modelo);

Matriz* costo_minimo(Matriz*modelo);

Matriz* aprox_vogel(Matriz*modelo);

double get_penalizacion_columna(NodoOrtogonal*columna);

double get_penalizacion_fila(NodoOrtogonal*fila);

Penalizacion* get_max_pen_fila(Matriz*modelo);

Penalizacion* get_max_pen_columna(Matriz*modelo);

int get_pos_min_fila(NodoOrtogonal*col,Lista*columnas);

int get_pos_min_columna(NodoOrtogonal*fila,Lista*filas);

int get_count_disponibles(Lista*l);

Matriz* opt_banquillo(Matriz*modelo);

int set_signos(Matriz**modelo);

Lista* get_costo_marginal(Matriz*modelo);

int do_path(Lista**l,NodoOrtogonal*ort,NodoOrtogonal*inicial,char dir);

int has_path(NodoOrtogonal*ort,NodoOrtogonal*inicial,char dir);

double get_min_negativo(Lista*l);

Matriz* opt_multip(Matriz*modelo);

int get_count_ocupados(Matriz*modelo);

int armar_gauss(Matriz*modelo,double(*mat)[],int cant);

int gauss_jordan(Matriz*modelo,double (*mat)[],int cant);

NodoOrtogonal* get_menor_cm(Matriz*modelo);

Matriz* asignacion(Matriz*modelo);

double get_costo_minimo_fila(NodoLista*fila);

double get_costo_minimo_columna(NodoLista*columna);

void solucion_inicial(Matriz*modelo);
#endif // FUNCIONESMODELOS_H_INCLUDED
