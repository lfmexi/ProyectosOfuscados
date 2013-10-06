#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtkextra/gtkextra.h>
#include "OperacionesEstructuras.c"

Matriz*mod=NULL;
GtkWidget*dialogo_pasos=NULL,*vbox=NULL,*notebook=NULL;

double get_costo_total(Matriz*modelo){

    if(modelo){
        double result=0;
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int i;
        NodoLista* recorrido=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal* recOrt=recorrido->ort;
            int j;
            for(j=0;j<columnas;j++){
                result=result+recOrt->contenido->asignado*recOrt->contenido->costo;
                recOrt=recOrt->derecha;
            }
            recorrido=recorrido->sig;
        }
        return result;
    }
    return 0;
}

static void fin(GtkWidget*widget,gpointer pointer){
    gtk_widget_destroy((GtkWidget*)pointer);
}

int set_signos(Matriz**modelo){
    if(*modelo){
        NodoLista*recf=(*modelo)->filas->inicio;
        while(recf){
            NodoOrtogonal*ort=recf->ort;
            while(ort){
                if(ort->contenido->asignado==0){
                    ort->contenido->signo='+';
                }
                ort=ort->derecha;
            }
            recf=recf->sig;
        }
        return 1;
    }
    return 0;
}

int has_path(NodoOrtogonal*ort,NodoOrtogonal*inicial,char dir){
    if(ort){
        if(dir=='i'){
            if(inicial->y==ort->y){
                if(inicial->x<ort->x){
                    return 1;
                }
            }else{
                NodoOrtogonal*recorrido=ort->izquierda;
                while(recorrido){
                    if(recorrido->contenido->asignado!=0){
                        if(recorrido->abajo!=NULL){
                            if(has_path(recorrido,inicial,'a')){
                                return 1;
                            }
                        }
                        if(recorrido->arriba!=NULL){
                            if(has_path(recorrido,inicial,'u')){
                                return 1;
                            }
                        }
                    }
                    recorrido=recorrido->izquierda;
                }
            }
        }else if(dir=='d'){
            if(inicial->y==ort->y){
                if(inicial->x>ort->x){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->derecha;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->abajo!=NULL){
                            if(has_path(rec,inicial,'a')){
                                return 1;
                            }
                        }
                        if(rec->arriba!=NULL){
                            if(has_path(rec,inicial,'u')){
                                return 1;
                            }
                        }
                    }
                    rec=rec->derecha;
                }
            }
        }else if(dir=='a'){
            if(inicial->x==ort->x){
                if(inicial->y>ort->y){
                    return 1;
                }
            }else{
                NodoOrtogonal*recorrido=ort->abajo;
                while(recorrido){
                    if(recorrido->contenido->asignado!=0){
                        if(recorrido->izquierda){
                            if(has_path(recorrido,inicial,'i')){
                                return 1;
                            }
                        }
                        if(recorrido->derecha){
                            if(has_path(recorrido,inicial,'d')){
                                return 1;
                            }
                        }
                    }
                    recorrido=recorrido->abajo;
                }
            }
        }else if(dir=='u'){
            if(inicial->x==ort->x){
                if(inicial->y<ort->y){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->arriba;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->izquierda){
                            if(has_path(rec,inicial,'i')){
                                return 1;
                            }
                        }
                        if(rec->derecha){
                            if(has_path(rec,inicial,'d')){
                                return 1;
                            }
                        }
                    }
                    rec=rec->arriba;
                }
            }
        }
    }
    return 0;
}

int do_path(Lista**l,NodoOrtogonal*ort,NodoOrtogonal*inicial,char dir){
    if((*l) && ort){
        if(dir=='s'){
            if(ort->izquierda){
                //int i=0;
                NodoOrtogonal*rec=ort->izquierda;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->abajo){
                            if(has_path(rec,inicial,'a')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'a');
                            }
                        }
                        if(rec->arriba){
                            if(has_path(rec,inicial,'u')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(l,rec,inicial,'u');
                            }
                        }
                    }
                    rec=rec->izquierda;
                }
            }
            if(ort->derecha){
                NodoOrtogonal*rec=ort->derecha;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->abajo){
                            if(has_path(rec,inicial,'a')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'a');
                            }
                        }
                        if(rec->arriba){
                            if(has_path(rec,inicial,'u')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(l,rec,inicial,'u');
                            }
                        }
                    }
                    rec=rec->derecha;
                }
            }
            if(ort->abajo){
                NodoOrtogonal*rec=ort->abajo;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->izquierda){
                            if(has_path(rec,inicial,'i')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'i');
                            }
                        }
                        if(rec->derecha){
                            if(has_path(rec,inicial,'d')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(l,rec,inicial,'d');
                            }
                        }
                    }
                    rec=rec->abajo;
                }
            }
            if(ort->arriba){
                NodoOrtogonal*rec=ort->arriba;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->izquierda){
                            if(has_path(rec,inicial,'i')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'i');
                            }
                        }
                        if(rec->derecha){
                            if(has_path(rec,inicial,'d')){
                                lista_add(&(*l),-rec->contenido->costo);
                                rec->contenido->signo='-';
                                (*l)->fin->ort=rec;
                                return do_path(l,rec,inicial,'d');
                            }
                        }
                    }
                    rec=rec->arriba;
                }
            }
        }else if(dir=='i'){
            if(inicial->y==ort->y){
                if(inicial->x<ort->x){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->izquierda;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->abajo){
                            if(has_path(rec,inicial,'a')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'a');
                            }
                        }
                        if(rec->arriba){
                            if(has_path(rec,inicial,'u')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'u');
                            }
                        }
                    }
                    rec=rec->izquierda;
                }
            }
        }else if(dir=='d'){
            if(inicial->y==ort->y){
                if(inicial->x>ort->x){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->derecha;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->abajo){
                            if(has_path(rec,inicial,'a')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'a');
                            }
                        }
                        if(rec->arriba){
                            if(has_path(rec,inicial,'u')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'u');
                            }
                        }
                    }
                    rec=rec->derecha;
                }
            }
        }else if(dir=='a'){
            if(inicial->x==ort->x){
                if(inicial->y>ort->y){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->abajo;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->izquierda){
                            if(has_path(rec,inicial,'i')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'i');
                            }
                        }
                        if(rec->derecha){
                            if(has_path(rec,inicial,'d')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'d');
                            }
                        }
                    }
                    rec=rec->abajo;
                }
            }
        }else if(dir=='u'){
            if(inicial->x==ort->x){
                if(inicial->y<ort->y){
                    return 1;
                }
            }else{
                NodoOrtogonal*rec=ort->arriba;
                while(rec){
                    if(rec->contenido->asignado!=0){
                        if(rec->izquierda){
                            if(has_path(rec,inicial,'i')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'i');
                            }
                        }
                        if(rec->derecha){
                            if(has_path(rec,inicial,'d')){
                                if((*l)->fin->valor>=0){
                                    rec->contenido->signo='-';
                                    lista_add(&(*l),-rec->contenido->costo);
                                }else{
                                    lista_add(&(*l),rec->contenido->costo);
                                    rec->contenido->signo='+';
                                }
                                (*l)->fin->ort=rec;
                                return do_path(&(*l),rec,inicial,'d');
                            }
                        }
                    }
                    rec=rec->arriba;
                }
            }
        }
    }
    return 0;
}

Lista* get_costo_marginal(Matriz*modelo){
    if(modelo){
        NodoLista*recf=modelo->filas->inicio;
        Lista*retorno=NULL;
        while(recf){
            NodoOrtogonal*ort=recf->ort;
            while(ort){
                if(ort->contenido->asignado==0){
                    Lista*actual=NULL;
                    lista_add(&actual,ort->contenido->costo);
                    actual->inicio->ort=ort;
                    do_path(&actual,ort,ort,'s');
                    double suma=lista_get_sum(actual);
                    if(suma<0){
                        if(retorno==NULL){
                            retorno=actual;
                        }else{
                            double sumaRet=lista_get_sum(retorno);
                            if(sumaRet>suma){
                                retorno=actual;
                            }else if(sumaRet==suma){
                                if(actual->inicio->valor<retorno->inicio->valor){
                                    retorno=actual;
                                }
                            }
                        }
                    }
                }
                ort=ort->derecha;
            }
            recf=recf->sig;
        }
        return retorno;
    }
    return NULL;
}

double get_min_negativo(Lista*l){
    if(l){
        NodoLista*rec=l->inicio;
        double min=0;
        while(rec){
            if(rec->valor<0){
                if(min==0){
                    min=rec->ort->contenido->asignado;
                }else if(rec->ort->contenido->asignado<min){
                    min=rec->ort->contenido->asignado;
                }
            }
            rec=rec->sig;
        }
        return min;
    }
    return -1;
}

Matriz* opt_banquillo(Matriz*modelo){
    if(modelo){
        set_signos(&modelo);
        while(1){
            Lista*marginal=get_costo_marginal(modelo);
            if(marginal){
                double minimo=get_min_negativo(marginal);
                NodoLista*rec=marginal->inicio;
                while(rec){
                    if(rec->valor>=0){
                        rec->ort->contenido->asignado=rec->ort->contenido->asignado+minimo;
                    }else{
                        rec->ort->contenido->asignado=rec->ort->contenido->asignado-minimo;
                    }
                    rec=rec->sig;
                }
            }else{
                return modelo;
            }
        }
    }
    return NULL;
}

int get_count_ocupados(Matriz*modelo){
    if(modelo){
        int i=0;
        NodoLista*rec=modelo->filas->inicio;
        while(rec){
            NodoOrtogonal*ort=rec->ort;
            while(ort){
                if(ort->contenido->asignado!=0){
                    i++;
                }
                ort=ort->derecha;
            }
            rec=rec->sig;
        }
        return i;
    }
    return -1;
}

int armar_gauss(Matriz*modelo,double (*mat)[get_size_lista(modelo->filas)+get_size_lista(modelo->columnas)],int cant){
    if(modelo){
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int i;
        int k=0;
        NodoLista*recf=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal*ort=recf->ort;
            while(ort){
                if(ort->contenido->asignado!=0){
                    if(ort->y!=0){
                        int y=ort->y-1;
                        mat[k][y]=1;
                    }
                    int x=ort->x+filas-1;
                    int last=filas+columnas-1;
                    mat[k][x]=1;
                    mat[k][last]=ort->contenido->costo;
                    k++;
                }
                ort=ort->derecha;
            }
            recf=recf->sig;
        }
    }
    return 0;
}

int gauss_jordan(Matriz*modelo,double (*mat)[get_size_lista(modelo->filas)+get_size_lista(modelo->columnas)],int cant){
    if(mat){
        int indice=0;
        int col=0;
        int reinicio=0;
        int tam=get_size_lista(modelo->filas)+get_size_lista(modelo->columnas);
        while(mat[indice][col]==0){
            int i;
            for(i=indice+1;i<cant;i++){
                if(mat[i][col]!=0){
                    int j;
                    for(j=0;j<tam;j++){
                        double temp=mat[indice][j];
                        mat[indice][j]=mat[i][j];
                        mat[i][j]=temp;
                    }
                    reinicio=indice+1;
                    i=cant;
                }
            }
            indice++;
            if(indice>=cant){
                if(col<tam && reinicio<cant){
                    indice=reinicio;
                    col++;
                }else{
                    break;
                }
            }
        }

        int i;
        int k=0;
        for(i=0;i<cant;i++){
            if(mat[i][k]!=1){
                if(mat[i][k]!=0){
                    int j;
                    double div=mat[i][k];
                    for(j=k;j<tam;j++){
                        mat[i][j]=mat[i][j]/div;
                    }
                }else{
                    indice=i;
                    while(mat[indice][k]==0){
                        int l;
                        for(l=indice+1;l<cant;l++){
                            if(mat[l][k]!=0){
                                int j;
                                for(j=0;j<tam;j++){
                                    double temp=mat[l][j];
                                    mat[l][j]=mat[indice][j];
                                    mat[indice][j]=temp;
                                }
                                l=cant;
                            }
                        }
                        indice++;
                        if(indice>=cant){break;}
                    }
                }
            }

            int l;

            for(l=i+1;l<cant;l++){
                if(mat[l][k]!=0){
                    int j;
                    double div=mat[l][k];
                    for(j=k;j<tam;j++){
                        double temp=mat[l][j]*(1/div);
                        double res=mat[i][j]-temp;
                        mat[l][j]=res;
                    }
                }
            }

            k++;
        }

        k=cant-1;
        for(i=cant-1;i>=0;i--){
            int l;
            for(l=i-1;l>=0;l--){
                if(mat[l][k]!=0){
                    double mult=mat[l][k];
                    int j;
                    for(j=tam-1;j>=k;j--){
                        double temp=mat[i][j]*mult;
                        double res=mat[l][j]-temp;
                        mat[l][j]=res;
                    }
                }
            }
            k--;
        }
    }
    return 0;
}

NodoOrtogonal* get_menor_cm(Matriz*modelo){
    if(modelo){
        NodoOrtogonal*ret=NULL;
        NodoLista*rec=modelo->filas->inicio;
        while(rec){
            NodoOrtogonal*ort=rec->ort;
            while(ort){
                if(ort->contenido->asignado==0){
                    if(ort->contenido->costo_marginal<0){
                        if(ret==NULL){
                            ret=ort;
                        }else if(ret->contenido->costo_marginal>ort->contenido->costo_marginal){
                            ret=ort;
                        }else if(ret->contenido->costo_marginal==ort->contenido->costo_marginal){
                            if(ret->contenido->costo>ort->contenido->costo){
                                ret=ort;
                            }
                        }
                    }
                }
                ort=ort->derecha;
            }
            rec=rec->sig;
        }
        return ret;
    }
    return NULL;
}

Matriz* opt_multip(Matriz*modelo){
    if(modelo){
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int cant=get_count_ocupados(modelo);
        while(1){
            double mat[cant][filas+columnas];
            int i=0;
            for(i=0;i<cant;i++){
                int j=0;
                for(j=0;j<(filas+columnas);j++){
                    mat[i][j]=0;
                }
            }
            armar_gauss(modelo,mat,cant);
            gauss_jordan(modelo,mat,cant);
            NodoLista*recorrido=modelo->filas->inicio;
            recorrido->coef=0;
            recorrido=recorrido->sig;
            int k=0;
            while(recorrido){
                recorrido->coef=mat[k][filas+columnas-1];
                recorrido=recorrido->sig;
                k++;
            }
            recorrido=modelo->columnas->inicio;
            while(recorrido){
                recorrido->coef=mat[k][filas+columnas-1];
                recorrido=recorrido->sig;
                k++;
            }
            recorrido=modelo->filas->inicio;
            while(recorrido){
                NodoLista*rec=modelo->columnas->inicio;
                NodoOrtogonal*ort=recorrido->ort;
                while(rec){
                    if(ort->contenido->asignado==0){
                        ort->contenido->costo_marginal=ort->contenido->costo-rec->coef-recorrido->coef;
                    }
                    rec=rec->sig;
                    ort=ort->derecha;
                }
                recorrido=recorrido->sig;
            }
            NodoOrtogonal*vacio=get_menor_cm(modelo);
            if(vacio!=NULL){
                Lista*actual=NULL;
                lista_add(&actual,vacio->contenido->costo);
                actual->inicio->ort=vacio;
                int x=do_path(&actual,vacio,vacio,'s');
                if(x){
                    double minimo=get_min_negativo(actual);
                    NodoLista*rec=actual->inicio;
                    while(rec){
                        if(rec->valor>=0){
                            rec->ort->contenido->asignado=rec->ort->contenido->asignado+minimo;
                        }else{
                            rec->ort->contenido->asignado=rec->ort->contenido->asignado-minimo;
                        }
                        rec=rec->sig;
                    }
                    printf("\n\n");
                }else{
                    return modelo;
                }
            }else{
                return modelo;
            }
        }
    }
    return NULL;
}

static void banquillo(GtkWidget*widget,gpointer pointer){
    gtk_widget_destroy((GtkWidget*)pointer);
    if(mod!=NULL){
        opt_banquillo(mod);
        int filas=get_size_lista(mod->filas);
        int columnas=get_size_lista(mod->columnas);

        GtkWidget*ventana_nuevo=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(ventana_nuevo),"Solucion por Banquillo");
        gtk_widget_set_size_request(GTK_WIDGET(ventana_nuevo),600,400);

        GtkWidget*scrolled2=gtk_scrolled_window_new(NULL,NULL);
        gtk_container_set_border_width(GTK_CONTAINER(scrolled2),10);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled2),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),scrolled2,TRUE,TRUE,0);

        GtkWidget*hbox=gtk_hbox_new(FALSE,0);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),hbox,TRUE,TRUE,0);

        char c[20];
        sprintf(c,"%.2f",get_costo_total(mod));
        GtkWidget*label=gtk_label_new("Costo total ");
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        label=gtk_label_new(c);
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        GtkWidget*sheet=gtk_sheet_new_browser(filas,columnas,"Solucion Final");
        gtk_container_add(GTK_CONTAINER(scrolled2),sheet);

        GtkWidget*cancel=gtk_button_new_with_label("Finalizar");
        g_signal_connect(cancel,"clicked",G_CALLBACK(fin),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),cancel,TRUE,TRUE,0);

        int i;
        NodoLista*rec=mod->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal*ort=rec->ort;
            int j;
            for(j=0;j<columnas;j++){
                if(ort->contenido->asignado!=0){
                    char c[20];
                    sprintf(c,"%.0f",ort->contenido->asignado);
                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),i,j,c);
                }
                ort=ort->derecha;
            }
            rec=rec->sig;
        }
        mod=NULL;
        gtk_widget_show_all(ventana_nuevo);
    }
}

static void multiplicadores(GtkWidget*widget,gpointer pointer){
    gtk_widget_destroy((GtkWidget*)pointer);
    if(mod!=NULL){
        opt_multip(mod);
        int filas=get_size_lista(mod->filas);
        int columnas=get_size_lista(mod->columnas);

        GtkWidget*ventana_nuevo=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(ventana_nuevo),"Solucion por Multiplicadores");
        gtk_widget_set_size_request(GTK_WIDGET(ventana_nuevo),600,400);

        GtkWidget*scrolled2=gtk_scrolled_window_new(NULL,NULL);
        gtk_container_set_border_width(GTK_CONTAINER(scrolled2),10);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled2),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),scrolled2,TRUE,TRUE,0);

        GtkWidget*hbox=gtk_hbox_new(FALSE,0);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),hbox,TRUE,TRUE,0);

        char c[20];
        sprintf(c,"%.2f",get_costo_total(mod));
        GtkWidget*label=gtk_label_new("Costo total ");
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        label=gtk_label_new(c);
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        GtkWidget*sheet=gtk_sheet_new_browser(filas,columnas,"Solucion Final");
        gtk_container_add(GTK_CONTAINER(scrolled2),sheet);

        GtkWidget*cancel=gtk_button_new_with_label("Finalizar");
        g_signal_connect(cancel,"clicked",G_CALLBACK(fin),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),cancel,TRUE,TRUE,0);

        int i;
        NodoLista*rec=mod->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal*ort=rec->ort;
            int j;
            for(j=0;j<columnas;j++){
                if(ort->contenido->asignado!=0){
                    char c[20];
                    sprintf(c,"%.0f",ort->contenido->asignado);
                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),i,j,c);
                }
                ort=ort->derecha;
            }
            rec=rec->sig;
        }
        mod=NULL;
        gtk_widget_show_all(ventana_nuevo);
    }
}

void solucion_inicial(Matriz*modelo,char*metodo){
    if(modelo){
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);

        GtkWidget*ventana_nuevo=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(ventana_nuevo),metodo);
        gtk_widget_set_size_request(GTK_WIDGET(ventana_nuevo),600,400);

        GtkWidget*scrolled2=gtk_scrolled_window_new(NULL,NULL);
        gtk_container_set_border_width(GTK_CONTAINER(scrolled2),10);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled2),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),scrolled2,TRUE,TRUE,0);

        GtkWidget*hbox=gtk_hbox_new(FALSE,0);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),hbox,TRUE,TRUE,0);

        char c[20];
        sprintf(c,"%.2f",get_costo_total(modelo));
        GtkWidget*label=gtk_label_new("Costo total ");
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        label=gtk_label_new(c);
        gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

        GtkWidget*sheet=gtk_sheet_new_browser(filas,columnas,"Solucion Inicial");
        gtk_container_add(GTK_CONTAINER(scrolled2),sheet);

        GtkWidget*boton=gtk_button_new_with_label("Banquillo");
        g_signal_connect(boton,"clicked",G_CALLBACK(banquillo),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),boton,TRUE,TRUE,0);

        GtkWidget*sig=gtk_button_new_with_label("Multiplicadores");
        g_signal_connect(sig,"clicked",G_CALLBACK(multiplicadores),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),sig,TRUE,TRUE,0);

        GtkWidget*cancel=gtk_button_new_with_label("Finalizar");
        g_signal_connect(cancel,"clicked",G_CALLBACK(fin),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),cancel,TRUE,TRUE,0);

        int i;
        NodoLista*rec=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoOrtogonal*ort=rec->ort;
            int j;
            for(j=0;j<columnas;j++){
                if(ort->contenido->asignado!=0){
                    char c[20];
                    sprintf(c,"%.0f",ort->contenido->asignado);
                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),i,j,c);
                }
                ort=ort->derecha;
            }
            rec=rec->sig;
        }
        mod=modelo;
        gtk_widget_show_all(ventana_nuevo);
    }
}

int agrega(Matriz**modelo){
    double sumDemanda=lista_get_sum((*modelo)->columnas);
    double sumOferta=lista_get_sum((*modelo)->filas);
    if(sumDemanda<sumOferta){
        double nuevoVal=sumOferta-sumDemanda;
        addColumna(&(*modelo),nuevoVal);
        return 1;
    }else if(sumDemanda>sumOferta){
        double nuevoVal=sumDemanda-sumOferta;
        addFila(&(*modelo),nuevoVal);
        return 1;
    }
    return 0;
}

Punto* get_pos_minimo(Matriz*modelo){
    if(modelo){
        Punto*p=(Punto*)malloc(sizeof(Punto));
        p->x=-1;
        p->y=-1;
        double costo=-1,oferta=-1,demanda=-1;
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        int i;
        NodoLista*rec=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            NodoLista*recc=modelo->columnas->inicio;
            NodoOrtogonal*ort=rec->ort;
            if(rec->valor>0){
                int j;
                for(j=0;j<columnas;j++){
                    if(ort->contenido->utilizable){
                        if(costo==-1){
                            p->x=j;
                            p->y=i;
                            costo=ort->contenido->costo;
                            oferta=rec->valor;demanda=recc->valor;
                        }else if(costo>ort->contenido->costo){
                            costo=ort->contenido->costo;
                            oferta=rec->valor;demanda=recc->valor;
                            p->x=j;p->y=i;
                        }else if(costo==ort->contenido->costo){
                            if(oferta<rec->valor || demanda<recc->valor){
                                costo=ort->contenido->costo;
                                oferta=rec->valor;demanda=recc->valor;
                                p->x=j;p->y=i;
                            }
                        }
                    }
                    recc=recc->sig;
                    ort=ort->derecha;
                }
            }
            rec=rec->sig;
        }
        if(p->x!=-1 && p->y!=-1){
            return p;
        }
    }
    return NULL;
}

Matriz* costo_minimo(Matriz*modelo,int de_vogel){
    if(modelo){
        if(!de_vogel){
            dialogo_pasos=gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title(GTK_WINDOW(dialogo_pasos),"Solución por Costo Mínimo");
            gtk_widget_set_size_request(GTK_WIDGET(dialogo_pasos),600,700);

            notebook=gtk_notebook_new();
            gtk_container_add(GTK_CONTAINER(dialogo_pasos),notebook);

        }
        agrega(&modelo);
        int cantidadPasos=0;
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        while(1){
            Punto*p=get_pos_minimo(modelo);
            if(p){
                NodoLista*recf=modelo->filas->inicio;
                int i=0;
                while(i<p->y){
                    recf=recf->sig;
                    i++;
                }
                NodoOrtogonal*ort=recf->ort;
                NodoLista*recc=modelo->columnas->inicio;
                i=0;
                while(i<p->x){
                    ort=ort->derecha;
                    recc=recc->sig;
                    i++;
                }
                if(recf->valor>recc->valor){
                    ort->contenido->asignado=recc->valor;
                    recf->valor=recf->valor-recc->valor;
                    recc->valor=0;
                    NodoOrtogonal*aux=recc->ort;
                    while(aux){
                        aux->contenido->utilizable=0;
                        aux=aux->abajo;
                    }
                }else if(recf->valor<recc->valor){
                    ort->contenido->asignado=recf->valor;
                    recc->valor=recc->valor-recf->valor;
                    recf->valor=0;
                    NodoOrtogonal*aux=recf->ort;
                    while(aux){
                        aux->contenido->utilizable=0;
                        aux=aux->derecha;
                    }
                }else{
                    ort->contenido->asignado=recc->valor;
                    recf->valor=0;
                    recc->valor=0;
                    NodoOrtogonal*aux=recc->ort;
                    while(aux){
                        aux->contenido->utilizable=0;
                        aux=aux->abajo;
                    }
                    aux=recf->ort;
                    while(aux){
                        aux->contenido->utilizable=0;
                        aux=aux->derecha;
                    }
                }
                cantidadPasos++;
                char c[25];
                sprintf(c,"Paso %d",cantidadPasos);
                GtkWidget*frame=gtk_frame_new(c);
                gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
                gtk_widget_set_size_request (frame, 500, 600);
                vbox=gtk_vbox_new(FALSE,1);
                gtk_container_add(GTK_CONTAINER(frame),vbox);

                GtkWidget*label=gtk_label_new(c);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

                GtkWidget*scroll=gtk_scrolled_window_new(NULL,NULL);
                gtk_container_set_border_width(GTK_CONTAINER(scroll),1);
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
                gtk_box_pack_start(GTK_BOX(vbox),scroll,TRUE,TRUE,0);

                GtkWidget*sheet=gtk_sheet_new_browser(filas+1,columnas+1," ");
                gtk_container_add(GTK_CONTAINER(scroll),sheet);
                gtk_sheet_row_button_add_label(GTK_SHEET(sheet),filas,"Demanda");
                gtk_sheet_column_button_add_label(GTK_SHEET(sheet),columnas,"Oferta");
                NodoLista*rec=modelo->filas->inicio;
                NodoLista*rec2=modelo->columnas->inicio;
                int x=0;
                while(rec){
                    NodoOrtogonal*ort=rec->ort;
                    while(ort){
                        if(ort->contenido->asignado!=0){
                            char c[20];
                            sprintf(c,"%.0f | %.0f",ort->contenido->asignado,ort->contenido->costo);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),ort->y,ort->x,c);
                        }else{
                            char c[20];
                            sprintf(c,"%.0f | %.0f",ort->contenido->asignado,ort->contenido->costo);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),ort->y,ort->x,c);
                        }
                        if(x==0){
                            char c2[20];
                            sprintf(c2,"%.0f",rec2->valor);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),filas,ort->x,c2);
                            rec2=rec2->sig;
                        }
                        ort=ort->derecha;
                    }
                    char c[20];
                    sprintf(c,"%.0f",rec->valor);
                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),x,columnas,c);
                    x++;
                    rec=rec->sig;
                }
            }else{
                break;
            }
        }
        if(de_vogel)
            solucion_inicial(modelo,"Solución por Vogel");
        else
            solucion_inicial(modelo,"Solución por Costo mínimo");

        gtk_widget_show_all(dialogo_pasos);
        return modelo;
    }
    return NULL;
}

Matriz*esquina_noroeste(Matriz*modelo){
    if(modelo){

        dialogo_pasos=gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(dialogo_pasos),"Solucion inicial esquina noroeste");
        gtk_widget_set_size_request(GTK_WIDGET(dialogo_pasos),600,700);

        notebook=gtk_notebook_new();
        gtk_container_add(GTK_CONTAINER(dialogo_pasos),notebook);

        agrega(&(modelo));
        int filas=get_size_lista((modelo)->filas);
        int columnas=get_size_lista((modelo)->columnas);
        int i=0;
        int j=0;
        int cantidadPasos=0;
        NodoLista*recorridof=(modelo)->filas->inicio;
        for(i=0;i<filas;i++){
            NodoLista*recorridoc=(modelo)->columnas->inicio;
            NodoOrtogonal*recorridort=recorridof->ort;
            for(j=0;j<columnas;j++){
                if(recorridoc->valor>0 && recorridof->valor>0){
                    if(recorridort->contenido->utilizable){
                        if(recorridof->valor>recorridoc->valor){
                            recorridort->contenido->asignado=recorridoc->valor;
                            recorridof->valor=recorridof->valor-recorridoc->valor;
                            recorridoc->valor=0;
                            NodoOrtogonal*recorridoAux=recorridoc->ort;
                            while(recorridoAux){
                                recorridoAux->contenido->utilizable=0;
                                recorridoAux=recorridoAux->abajo;
                            }
                            recorridoc=recorridoc->sig;
                            recorridort=recorridort->derecha;
                        }else if(recorridof->valor<recorridoc->valor){
                            recorridort->contenido->asignado=recorridof->valor;
                            recorridoc->valor=recorridoc->valor-recorridof->valor;
                            recorridof->valor=0;
                            NodoOrtogonal*recorridoAux=recorridof->ort;
                            while(recorridoAux){
                                recorridoAux->contenido->utilizable=0;
                                recorridoAux=recorridoAux->derecha;
                            }
                            j=columnas;
                        }else{
                            recorridort->contenido->asignado=recorridof->valor;
                            recorridoc->valor=0;
                            recorridof->valor=0;
                            NodoOrtogonal*recorridoAux=recorridof->ort;
                            while(recorridoAux){
                                recorridoAux->contenido->utilizable=0;
                                recorridoAux=recorridoAux->derecha;
                            }
                            recorridoAux=recorridoc->ort;
                            while(recorridoAux){
                                recorridoAux->contenido->utilizable=0;
                                recorridoAux=recorridoAux->abajo;
                            }
                            j=columnas;
                        }
                        cantidadPasos++;
                        char c[25];
                        sprintf(c,"Paso %d",cantidadPasos);
                        GtkWidget*frame=gtk_frame_new(c);
                        gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
                        gtk_widget_set_size_request (frame, 500, 600);

                        vbox=gtk_vbox_new(FALSE,1);
                        gtk_container_add(GTK_CONTAINER(frame),vbox);


                        GtkWidget*label=gtk_label_new(c);
                        gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

                        GtkWidget*scroll=gtk_scrolled_window_new(NULL,NULL);
                        gtk_container_set_border_width(GTK_CONTAINER(scroll),1);
                        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
                        gtk_box_pack_start(GTK_BOX(vbox),scroll,TRUE,TRUE,0);

                        GtkWidget*sheet=gtk_sheet_new_browser(filas+1,columnas+1," ");
                        gtk_container_add(GTK_CONTAINER(scroll),sheet);
                        gtk_sheet_row_button_add_label(GTK_SHEET(sheet),filas,"Demanda");
                        gtk_sheet_column_button_add_label(GTK_SHEET(sheet),columnas,"Oferta");
                        NodoLista*rec=modelo->filas->inicio;
                        NodoLista*rec2=modelo->columnas->inicio;
                        int x=0;
                        while(rec){
                            NodoOrtogonal*ort=rec->ort;
                            while(ort){
                                if(ort->contenido->asignado!=0){
                                    char c[20];
                                    sprintf(c,"%.0f",ort->contenido->asignado);
                                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),ort->y,ort->x,c);
                                }
                                if(x==0){
                                    char c2[20];
                                    sprintf(c2,"%.0f",rec2->valor);
                                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),filas,ort->x,c2);
                                    rec2=rec2->sig;
                                }
                                ort=ort->derecha;
                            }
                            char c[20];
                            sprintf(c,"%.0f",rec->valor);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),x,columnas,c);
                            x++;
                            rec=rec->sig;
                        }
                    }else{
                        recorridort=recorridort->derecha;
                        recorridoc=recorridoc->sig;
                    }
                }else{
                    recorridort=recorridort->derecha;
                    recorridoc=recorridoc->sig;
                }
            }
            recorridof=recorridof->sig;
        }
        gtk_widget_show_all(dialogo_pasos);
        solucion_inicial(modelo,"Solucion inicial Esquina Noroeste");
        return modelo;
    }
    return NULL;
}

double get_penalizacion_columna(NodoOrtogonal*columna){
    if(columna){
        double costo1=-1,costo2=-1;
        NodoOrtogonal*ort=columna;
        while(ort){
            if(ort->contenido->utilizable){
                if(costo1==-1){
                    costo1=ort->contenido->costo;
                }else if(ort->contenido->costo>costo1 && costo2==-1){
                    costo2=ort->contenido->costo;
                }else if(ort->contenido->costo>costo1 && ort->contenido->costo<costo2){
                    costo2=ort->contenido->costo;
                }else if(ort->contenido->costo<costo1){
                    costo2=costo1;
                    costo1=ort->contenido->costo;
                }
            }
            ort=ort->abajo;
        }
        if(costo1!=-1 && costo2!=-1){
            return costo2-costo1;
        }
    }
    return -1;
}

double get_penalizacion_fila(NodoOrtogonal*fila){
    if(fila){
        double costo1=-1,costo2=-1;
        NodoOrtogonal*ort=fila;
        while(ort){
            if(ort->contenido->utilizable){
                if(costo1==-1){
                    costo1=ort->contenido->costo;
                }else if(ort->contenido->costo>costo1 && costo2==-1){
                    costo2=ort->contenido->costo;
                }else if(ort->contenido->costo>costo1 && ort->contenido->costo<costo2){
                    costo2=ort->contenido->costo;
                }else if(ort->contenido->costo<costo1){
                    costo2=costo1;
                    costo1=ort->contenido->costo;
                }
            }
            ort=ort->derecha;
        }
        if(costo1!=-1 && costo2!=-1){
            return costo2-costo1;
        }
    }
    return -1;
}

int get_pos_min_fila(NodoOrtogonal*col,Lista*columnas){
    if(col){
        double costo=-1;
        double demanda=-1;
        int i=0;
        int res=-1;
        NodoLista*recorrido=columnas->inicio;
        while(col){
            if(col->contenido->utilizable){
                if(costo==-1){
                    costo=col->contenido->costo;
                    demanda=recorrido->valor;
                    res=i;
                }else if(col->contenido->costo<costo){
                    costo=col->contenido->costo;
                    demanda=recorrido->valor;
                    res=i;
                }else if(col->contenido->costo==costo){
                    if(recorrido->valor>demanda){
                        costo=col->contenido->costo;
                        demanda=recorrido->valor;
                        res=i;
                    }
                }
            }
            col=col->derecha;
            recorrido=recorrido->sig;
            i++;
        }
        return res;
    }
    return -1;
}

int get_pos_min_columna(NodoOrtogonal*fila,Lista*filas){
    if(fila){
        double costo=-1;
        double oferta=-1;
        int i=0;
        int res=-1;
        NodoLista*recorrido=filas->inicio;
        NodoOrtogonal*ort=fila;
        while(ort){
            if(ort->contenido->utilizable){
                if(costo==-1){
                    costo=ort->contenido->costo;
                    oferta=recorrido->valor;
                    res=i;
                }else if(ort->contenido->costo<costo){
                    costo=ort->contenido->costo;
                    oferta=recorrido->valor;
                    res=i;
                }else if(ort->contenido->costo==costo){
                    if(recorrido->valor>oferta){
                        costo=ort->contenido->costo;
                        oferta=recorrido->valor;
                        res=i;
                    }
                }
            }
            ort=ort->abajo;
            recorrido=recorrido->sig;
            i++;
        }
        return res;
    }
    return -1;
}

Penalizacion* get_max_pen_fila(Matriz*modelo){
    if(modelo){
        Penalizacion*p=(Penalizacion*)malloc(sizeof(Penalizacion));
        int pos=-1;
        double min=-1;
        int i=0;
        NodoLista*recorrido=modelo->filas->inicio;
        while(recorrido){
            if(recorrido->utilizable){
                double pen=get_penalizacion_fila(recorrido->ort);
                if(min==-1){
                    min=pen;
                    pos=i;
                }else if(pen>min){
                    min=pen;
                    pos=i;
                }else if(pen==min){
                    int posMin=get_pos_min_fila(recorrido->ort,modelo->columnas);
                    NodoLista*rec=modelo->columnas->inicio;
                    int j;
                    for(j=0;j<posMin;j++){
                        rec=rec->sig;
                    }
                    double demanda1=rec->valor;
                    double oferta1=recorrido->valor;
                    NodoLista*aux=modelo->filas->inicio;
                    for(j=0;j<pos;j++){
                        aux=aux->sig;
                    }
                    posMin=get_pos_min_fila(aux->ort,modelo->columnas);
                    rec=modelo->columnas->inicio;
                    for(j=0;j<posMin;j++){
                        rec=rec->sig;
                    }
                    double demanda2=rec->valor;
                    double oferta2=aux->valor;
                    if(demanda1>=demanda2 && oferta1>=oferta2){
                        min=pen;
                        pos=i;
                    }
                }
            }
            recorrido=recorrido->sig;
            i++;
        }
        p->p=min;
        p->pos=pos;
        return p;
    }
    return NULL;
}

Penalizacion* get_max_pen_columna(Matriz*modelo){
    if(modelo){
        Penalizacion*p=(Penalizacion*)malloc(sizeof(Penalizacion));
        int pos=-1;
        double min=-1;
        int i=0;
        NodoLista*recorrido=modelo->columnas->inicio;
        while(recorrido){
            if(recorrido->utilizable){
                double pen=get_penalizacion_columna(recorrido->ort);
                if(min==-1){
                    min=pen;
                    pos=i;
                }else if(pen>min){
                    min=pen;
                    pos=i;
                }else if(pen==min){
                    int posMin=get_pos_min_columna(recorrido->ort,modelo->filas);
                    NodoLista*rec=modelo->filas->inicio;
                    int j;
                    for(j=0;j<posMin;j++){
                        rec=rec->sig;
                    }
                    double oferta1=rec->valor;
                    double demanda1=recorrido->valor;
                    NodoLista*aux=modelo->columnas->inicio;
                    for(j=0;j<pos;j++){
                        aux=aux->sig;
                    }
                    posMin=get_pos_min_columna(aux->ort,modelo->filas);
                    rec=modelo->filas->inicio;
                    for(j=0;j<posMin;j++){
                        rec=rec->sig;
                    }
                    double oferta2=rec->valor;
                    double demanda2=aux->valor;
                    if(demanda1>=demanda2 && oferta1>=oferta2){
                        min=pen;
                        pos=i;
                    }
                }
            }
            recorrido=recorrido->sig;
            i++;
        }
        p->p=min;
        p->pos=pos;
        return p;
    }
    return NULL;
}

int get_count_disponibles(Lista*l){
    if(l){
        int i=0;
        NodoLista*recorrido=l->inicio;
        while(recorrido){
            if(recorrido->utilizable)
                i++;
            recorrido=recorrido->sig;
        }
        return i;
    }
    return 0;
}

Matriz* aprox_vogel(Matriz*modelo){
    if(modelo){
        dialogo_pasos=gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(dialogo_pasos),"Solución por Aproximación de Vogel");
        gtk_widget_set_size_request(GTK_WIDGET(dialogo_pasos),600,700);
        notebook=gtk_notebook_new();
        gtk_container_add(GTK_CONTAINER(dialogo_pasos),notebook);
        agrega(&modelo);
        int cantidadPasos=0;
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        while(1){
            int filas_disp=get_count_disponibles(modelo->filas);
            int col_disp=get_count_disponibles(modelo->columnas);
            if(filas_disp>1 && col_disp>1){
                cantidadPasos++;
                char c[25];
                sprintf(c,"Paso %d",cantidadPasos);
                GtkWidget*frame=gtk_frame_new(c);
                gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
                gtk_widget_set_size_request (frame, 500, 600);
                vbox=gtk_vbox_new(FALSE,1);
                gtk_container_add(GTK_CONTAINER(frame),vbox);

                GtkWidget*label=gtk_label_new(c);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

                GtkWidget*scroll=gtk_scrolled_window_new(NULL,NULL);
                gtk_container_set_border_width(GTK_CONTAINER(scroll),1);
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
                gtk_box_pack_start(GTK_BOX(vbox),scroll,TRUE,TRUE,0);

                GtkWidget*sheet=gtk_sheet_new_browser(filas+1,columnas+1," ");
                gtk_container_add(GTK_CONTAINER(scroll),sheet);
                gtk_sheet_row_button_add_label(GTK_SHEET(sheet),filas,"Demanda");
                gtk_sheet_column_button_add_label(GTK_SHEET(sheet),columnas,"Oferta");

                Penalizacion*fila=get_max_pen_fila(modelo);
                Penalizacion*columna=get_max_pen_columna(modelo);
                if(fila->p>columna->p){
                    char c2[40];
                    sprintf(c2,"Fila tomada=%i, Penalización=%.0f",fila->pos,fila->p);
                    label=gtk_label_new(c2);
                    gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);
                    NodoLista*recorrido=modelo->filas->inicio;
                    int i;
                    for(i=0;i<fila->pos;i++){
                        recorrido=recorrido->sig;
                    }
                    int x=get_pos_min_fila(recorrido->ort,modelo->columnas);
                    NodoOrtogonal* ort=recorrido->ort;
                    NodoLista*rec=modelo->columnas->inicio;
                    for(i=0;i<x;i++){
                        rec=rec->sig;
                        ort=ort->derecha;
                    }
                    if(recorrido->valor>rec->valor){
                        ort->contenido->asignado=rec->valor;
                        recorrido->valor=recorrido->valor-rec->valor;
                        rec->valor=0;
                        NodoOrtogonal*aux=rec->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->abajo;
                        }
                        rec->utilizable=0;
                    }else if(recorrido->valor<rec->valor){
                        ort->contenido->asignado=recorrido->valor;
                        rec->valor=rec->valor-recorrido->valor;
                        recorrido->valor=0;
                        NodoOrtogonal*aux=recorrido->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->derecha;
                        }
                        recorrido->utilizable=0;
                    }else{
                        ort->contenido->asignado=rec->valor;
                        recorrido->valor=0;
                        rec->valor=0;
                        NodoOrtogonal*aux=rec->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->abajo;
                        }
                        aux=recorrido->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->derecha;
                        }
                        recorrido->utilizable=0;
                        rec->utilizable=0;
                    }
                }else if(fila->p<columna->p){
                    char c2[40];
                    sprintf(c2,"Columna tomada=%i, Penalización=%.0f",columna->pos,columna->p);
                    label=gtk_label_new(c2);
                    gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);
                    NodoLista*recorrido=modelo->columnas->inicio;
                    int i;
                    for(i=0;i<columna->pos;i++){
                        recorrido=recorrido->sig;
                    }
                    int x=get_pos_min_columna(recorrido->ort,modelo->filas);
                    NodoOrtogonal* ort=recorrido->ort;
                    NodoLista*rec=modelo->filas->inicio;
                    for(i=0;i<x;i++){
                        rec=rec->sig;
                        ort=ort->abajo;
                    }
                    if(recorrido->valor>rec->valor){
                        ort->contenido->asignado=rec->valor;
                        recorrido->valor=recorrido->valor-rec->valor;
                        rec->valor=0;
                        NodoOrtogonal*aux=rec->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->derecha;
                        }
                        rec->utilizable=0;
                    }else if(recorrido->valor<rec->valor){
                        ort->contenido->asignado=recorrido->valor;
                        rec->valor=rec->valor-recorrido->valor;
                        recorrido->valor=0;
                        NodoOrtogonal*aux=recorrido->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->abajo;
                        }
                        recorrido->utilizable=0;
                    }else{
                        ort->contenido->asignado=rec->valor;
                        recorrido->valor=0;
                        rec->valor=0;
                        NodoOrtogonal*aux=rec->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->derecha;
                        }
                        aux=recorrido->ort;
                        while(aux){
                            aux->contenido->utilizable=0;
                            aux=aux->abajo;
                        }
                        recorrido->utilizable=0;
                        rec->utilizable=0;
                    }

                }else{
                    NodoLista*recf=modelo->filas->inicio;
                    NodoLista*rec=modelo->columnas->inicio;

                    double oferta1;
                    double demanda1;
                    double costo1;
                    int i;
                    for(i=0;i<fila->pos;i++){
                        recf=recf->sig;
                    }
                    oferta1=recf->valor;
                    NodoOrtogonal*ort=recf->ort;
                    int x=get_pos_min_fila(recf->ort,modelo->columnas);
                    for(i=0;i<x;i++){
                        ort=ort->derecha;rec=rec->sig;
                    }
                    demanda1=rec->valor;
                    costo1=ort->contenido->costo;

                    recf=modelo->filas->inicio;
                    rec=modelo->columnas->inicio;
                    double oferta2,demanda2,costo2;
                    for(i=0;i<columna->pos;i++){
                        rec=rec->sig;
                    }
                    demanda2=rec->valor;
                    ort=rec->ort;
                    int y=get_pos_min_columna(ort,modelo->filas);
                    for(i=0;i<y;i++){
                        ort=ort->abajo;recf=recf->sig;
                    }
                    oferta2=recf->valor;
                    costo2=ort->contenido->costo;

                    if(costo2>costo1){
                        char c2[40];
                        sprintf(c2,"Fila tomada=%i, Penalización=%.0f",fila->pos,fila->p);
                        label=gtk_label_new(c2);
                        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

                        NodoLista*recorrido=modelo->filas->inicio;
                        for(i=0;i<fila->pos;i++){
                            recorrido=recorrido->sig;
                        }
                        x=get_pos_min_fila(recorrido->ort,modelo->columnas);
                        ort=recorrido->ort;
                        rec=modelo->columnas->inicio;
                        for(i=0;i<x;i++){
                            rec=rec->sig;
                            ort=ort->derecha;
                        }
                        if(recorrido->valor>rec->valor){
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=recorrido->valor-rec->valor;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            rec->utilizable=0;
                        }else if(recorrido->valor<rec->valor){
                            ort->contenido->asignado=recorrido->valor;
                            rec->valor=rec->valor-recorrido->valor;
                            recorrido->valor=0;
                            NodoOrtogonal*aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            recorrido->utilizable=0;
                        }else{
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=0;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            recorrido->utilizable=0;
                            rec->utilizable=0;
                        }
                    }else if(costo2<costo1){
                        char c2[40];
                        sprintf(c2,"Columna tomada=%i, Penalización=%.0f",columna->pos,columna->p);
                        label=gtk_label_new(c2);
                        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

                        NodoLista*recorrido=modelo->columnas->inicio;
                        for(i=0;i<columna->pos;i++){
                            recorrido=recorrido->sig;
                        }
                        x=get_pos_min_columna(recorrido->ort,modelo->filas);
                        ort=recorrido->ort;
                        rec=modelo->filas->inicio;
                        for(i=0;i<x;i++){
                            rec=rec->sig;
                            ort=ort->abajo;
                        }
                        if(recorrido->valor>rec->valor){
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=recorrido->valor-rec->valor;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            rec->utilizable=0;
                        }else if(recorrido->valor<rec->valor){
                            ort->contenido->asignado=recorrido->valor;
                            rec->valor=rec->valor-recorrido->valor;
                            recorrido->valor=0;
                            NodoOrtogonal*aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            recorrido->utilizable=0;
                        }else{
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=0;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            recorrido->utilizable=0;
                            rec->utilizable=0;
                        }
                    }else if(oferta1>oferta2 || demanda1>demanda2){
                        char c2[40];
                        sprintf(c2,"Fila tomada=%i, Penalización=%.0f",fila->pos,fila->p);
                        label=gtk_label_new(c2);
                        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

                        NodoLista*recorrido=modelo->filas->inicio;
                        for(i=0;i<fila->pos;i++){
                            recorrido=recorrido->sig;
                        }
                        x=get_pos_min_fila(recorrido->ort,modelo->columnas);
                        ort=recorrido->ort;
                        rec=modelo->columnas->inicio;
                        for(i=0;i<x;i++){
                            rec=rec->sig;
                            ort=ort->derecha;
                        }
                        if(recorrido->valor>rec->valor){
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=recorrido->valor-rec->valor;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            rec->utilizable=0;
                        }else if(recorrido->valor<rec->valor){
                            ort->contenido->asignado=recorrido->valor;
                            rec->valor=rec->valor-recorrido->valor;
                            recorrido->valor=0;
                            NodoOrtogonal*aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            recorrido->utilizable=0;
                        }else{
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=0;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            recorrido->utilizable=0;
                            rec->utilizable=0;
                        }
                    }else{
                        char c2[40];
                        sprintf(c2,"Columna tomada=%i, Penalización=%.0f",columna->pos,columna->p);
                        label=gtk_label_new(c2);
                        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

                        NodoLista*recorrido=modelo->columnas->inicio;
                        for(i=0;i<columna->pos;i++){
                            recorrido=recorrido->sig;
                        }
                        x=get_pos_min_columna(recorrido->ort,modelo->filas);
                        ort=recorrido->ort;
                        rec=modelo->filas->inicio;
                        for(i=0;i<x;i++){
                            rec=rec->sig;
                            ort=ort->abajo;
                        }
                        if(recorrido->valor>rec->valor){
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=recorrido->valor-rec->valor;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            rec->utilizable=0;
                        }else if(recorrido->valor<rec->valor){
                            ort->contenido->asignado=recorrido->valor;
                            rec->valor=rec->valor-recorrido->valor;
                            recorrido->valor=0;
                            NodoOrtogonal*aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            recorrido->utilizable=0;
                        }else{
                            ort->contenido->asignado=rec->valor;
                            recorrido->valor=0;
                            rec->valor=0;
                            NodoOrtogonal*aux=rec->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->derecha;
                            }
                            aux=recorrido->ort;
                            while(aux){
                                aux->contenido->utilizable=0;
                                aux=aux->abajo;
                            }
                            recorrido->utilizable=0;
                            rec->utilizable=0;
                        }
                    }
                }
                NodoLista*rec=modelo->filas->inicio;
                NodoLista*rec2=modelo->columnas->inicio;
                int x=0;
                while(rec){
                    NodoOrtogonal*ort=rec->ort;
                    while(ort){
                        if(ort->contenido->asignado!=0){
                            char c[20];
                            sprintf(c,"%.0f | %.0f",ort->contenido->asignado,ort->contenido->costo);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),ort->y,ort->x,c);
                        }else{
                            char c[20];
                            sprintf(c,"%.0f | %.0f",ort->contenido->asignado,ort->contenido->costo);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),ort->y,ort->x,c);
                        }
                        if(x==0){
                            char c2[20];
                            sprintf(c2,"%.0f",rec2->valor);
                            gtk_sheet_set_cell_text(GTK_SHEET(sheet),filas,ort->x,c2);
                            rec2=rec2->sig;
                        }
                        ort=ort->derecha;
                    }
                    char c[20];
                    sprintf(c,"%.0f",rec->valor);
                    gtk_sheet_set_cell_text(GTK_SHEET(sheet),x,columnas,c);
                    x++;
                    rec=rec->sig;
                }
            }else{
                break;
            }
        }
        return costo_minimo(modelo,1);
    }
    return NULL;
}


