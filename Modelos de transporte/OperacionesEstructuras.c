#include "Estructuras.h"
#include "FuncionesEstructuras.h"

int addColumna(Matriz**modelo,double valor){
    if((*modelo)){
        lista_add(&((*modelo)->columnas),valor);
        if((*modelo)->filas){
            if(get_size_lista((*modelo)->filas)>0){
                NodoOrtogonal*ort=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                ort->contenido=(Casilla*)malloc(sizeof(Casilla));
                ort->contenido->costo=0;
                ort->contenido->asignado=0;
                ort->contenido->utilizable=1;
                ort->contenido->optimo=0;
                ort->arriba=0;
                ort->abajo=0;
                int columnas=get_size_lista((*modelo)->columnas);
                ort->x=columnas-1;
                (*modelo)->columnas->fin->ort=ort;
                if(columnas>1){
                    NodoOrtogonal*ortAnt=(*modelo)->columnas->fin->ant->ort;
                    NodoLista*recorrido=(*modelo)->filas->inicio;
                    int y=0;
                    if(ortAnt){
                        while(recorrido){
                            ortAnt->derecha=ort;
                            ort->izquierda=ortAnt;
                            ort->derecha=0;
                            ort->y=y;
                            if(recorrido->sig){
                                NodoOrtogonal*nuevo=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                                nuevo->contenido=(Casilla*)malloc(sizeof(Casilla));
                                nuevo->contenido->costo=0;
                                nuevo->contenido->asignado=0;
                                nuevo->contenido->utilizable=1;
                                nuevo->contenido->optimo=0;
                                nuevo->x=columnas-1;
                                ort->abajo=nuevo;
                                nuevo->arriba=ort;
                                nuevo->abajo=0;
                                ort=nuevo;
                            }else{
                                ort->abajo=0;
                                ort=ort->abajo;
                            }
                            ortAnt=ortAnt->abajo;
                            recorrido=recorrido->sig;
                            y++;
                        }
                    }
                }else{
                    NodoLista*recorrido=(*modelo)->filas->inicio;
                    int y=0;
                    while(recorrido){
                        recorrido->ort=ort;
                        ort->y=y;
                        ort->izquierda=0;
                        ort->derecha=0;
                        if(recorrido->sig){
                            NodoOrtogonal*nuevo=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                            nuevo->contenido=(Casilla*)malloc(sizeof(Casilla));
                            nuevo->contenido->costo=0;
                            nuevo->contenido->asignado=0;
                            nuevo->contenido->utilizable=1;
                            nuevo->contenido->optimo=0;
                            nuevo->x=columnas-1;
                            ort->abajo=nuevo;
                            nuevo->arriba=ort;
                            nuevo->abajo=0;
                            ort=nuevo;
                        }else{
                            ort->abajo=0;
                            ort=ort->abajo;
                        }
                        recorrido=recorrido->sig;
                        y++;
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

int addFila(Matriz**modelo,double valor){
    if((*modelo)){
        lista_add(&((*modelo)->filas),valor);
        if((*modelo)->columnas){
            if(get_size_lista((*modelo)->columnas)>0){
                NodoOrtogonal*ort=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                ort->contenido=(Casilla*)malloc(sizeof(Casilla));
                ort->contenido->costo=0;
                ort->contenido->asignado=0;
                ort->contenido->utilizable=1;
                ort->contenido->optimo=0;
                ort->izquierda=0;
                ort->derecha=0;
                int filas=get_size_lista((*modelo)->filas);
                ort->y=filas-1;
                (*modelo)->filas->fin->ort=ort;
                if(filas>1){
                    NodoOrtogonal*ortAnt=(*modelo)->filas->fin->ant->ort;
                    NodoLista*recorrido=(*modelo)->columnas->inicio;
                    int x=0;
                    if(ortAnt){
                        while(recorrido){
                            ortAnt->abajo=ort;
                            ort->arriba=ortAnt;
                            ort->abajo=0;
                            ort->x=x;
                            if(recorrido->sig){
                                NodoOrtogonal*nuevo=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                                nuevo->contenido=(Casilla*)malloc(sizeof(Casilla));
                                nuevo->contenido->costo=0;
                                nuevo->contenido->asignado=0;
                                nuevo->contenido->utilizable=1;
                                nuevo->contenido->optimo=0;
                                nuevo->y=filas-1;
                                ort->derecha=nuevo;
                                nuevo->izquierda=ort;
                                nuevo->derecha=0;
                                ort=nuevo;
                            }else{
                                ort->derecha=0;
                                ort=ort->derecha;
                            }
                            ortAnt=ortAnt->derecha;
                            recorrido=recorrido->sig;
                            x++;
                        }
                    }
                }else{
                    NodoLista*recorrido=(*modelo)->columnas->inicio;
                    int x=0;
                    while(recorrido){
                        recorrido->ort=ort;
                        ort->arriba=0;
                        ort->abajo=0;
                        ort->x=x;
                        if(recorrido->sig){
                            NodoOrtogonal*nuevo=(NodoOrtogonal*)malloc(sizeof(NodoOrtogonal));
                            nuevo->contenido=(Casilla*)malloc(sizeof(Casilla));
                            nuevo->contenido->costo=0;
                            nuevo->contenido->asignado=0;
                            nuevo->contenido->utilizable=1;
                            nuevo->contenido->optimo=0;
                            nuevo->y=filas-1;
                            ort->derecha=nuevo;
                            nuevo->izquierda=ort;
                            nuevo->abajo=0;
                            nuevo->arriba=0;
                            nuevo->derecha=0;
                            ort=nuevo;
                        }else{
                            ort->derecha=0;
                            ort=ort->derecha;
                        }
                        recorrido=recorrido->sig;
                        x++;
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

int get_size_lista(Lista*l){
    if(l){
        int tam=0;
        NodoLista*recorrido=l->inicio;
        while(recorrido){
            tam++;
            recorrido=recorrido->sig;
        }
        return tam;
    }
    return 0;
}

int matriz_set_costo(Matriz**modelo,int x,int y,double costo){
    if(x>=0 && y>=0 && x<get_size_lista((*modelo)->columnas)
       && y<get_size_lista((*modelo)->filas)){
        NodoOrtogonal*recorrido=lista_getNodo((*modelo)->filas,y)->ort;
        if(recorrido){
            int i;
            for(i=0;i<x;i++){
                recorrido=recorrido->derecha;
            }
            if(recorrido){
                recorrido->contenido->costo=costo;
                return 1;
            }
        }
    }
    return 0;
}

int matriz_set_asignado(Matriz**modelo,int x,int y,double asignado){
    if(x>=0 && y>=0 && x<get_size_lista((*modelo)->columnas)
       && y<get_size_lista((*modelo)->filas)){
        NodoOrtogonal*recorrido=lista_getNodo((*modelo)->filas,y)->ort;
        if(recorrido){
            int i;
            for(i=0;i<x;i++){
                recorrido=recorrido->derecha;
            }
            if(recorrido){
                recorrido->contenido->asignado=asignado;
                return 1;
            }
        }
    }
    return 0;
}

int lista_add(Lista**l,double valor){
    if((*l)==0){
        (*l)=(Lista*)malloc(sizeof(Lista));
        (*l)->inicio=(NodoLista*)malloc(sizeof(NodoLista));
        (*l)->inicio->valor=valor;
        (*l)->inicio->utilizable=1;
        (*l)->inicio->ant=0;
        (*l)->inicio->sig=0;
        (*l)->inicio->ort=0;
        (*l)->fin=(*l)->inicio;
        return 1;
    }else{
        if((*l)->inicio==0){
            (*l)->inicio=(NodoLista*)malloc(sizeof(NodoLista));
            (*l)->inicio->valor=valor;
            (*l)->inicio->utilizable=1;
            (*l)->inicio->ant=0;
            (*l)->inicio->sig=0;
            (*l)->inicio->ort=0;
            (*l)->fin=(*l)->inicio;
            return 1;
        }else{
            NodoLista*nuevo=(NodoLista*)malloc(sizeof(NodoLista));
            nuevo->valor=valor;
            nuevo->utilizable=1;
            nuevo->ant=(*l)->fin;
            nuevo->sig=0;
            nuevo->ort=0;
            (*l)->fin->sig=nuevo;
            (*l)->fin=nuevo;
            return 1;
        }
    }
}

NodoLista* lista_getNodo(Lista*l,int i){
    if(l){
        if(i>=0 && i<get_size_lista(l)){
            NodoLista*recorrido=l->inicio;
            if(recorrido){
                int j;
                for(j=0;j<i;j++){
                    recorrido=recorrido->sig;
                }
                return recorrido;
            }
        }
    }
    return 0;
}

double lista_get_sum(Lista*l){
    double res=0;
    NodoLista*recorrido=l->inicio;
    while(recorrido){
        res=res+recorrido->valor;
        recorrido=recorrido->sig;
    }
    return res;
}

void imprimirLista(Lista*l){
    if(l){
        NodoLista*recorrido=l->inicio;
        while(recorrido){
            printf("%f\n",recorrido->coef);
            recorrido=recorrido->sig;
        }
    }
}

void imprimirCostos(Matriz*modelo){
    if(modelo){
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int i;
        NodoLista* recorrido=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal* recOrt=recorrido->ort;
            int j;
            for(j=0;j<columnas;j++){
                printf("| %f ",recOrt->contenido->costo);
                recOrt=recOrt->derecha;
            }
            printf("|\n");
            recorrido=recorrido->sig;
        }
    }
}

void imprimirMatriz(Matriz*modelo){
    if(modelo){
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int i;
        NodoLista* recorrido=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal* recOrt=recorrido->ort;
            int j;
            for(j=0;j<columnas;j++){
                printf("| %f ",recOrt->contenido->asignado);
                recOrt=recOrt->derecha;
            }
            printf("|\n");
            recorrido=recorrido->sig;
        }
    }
}
