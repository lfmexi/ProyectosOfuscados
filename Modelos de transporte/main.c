#include "Estructuras.h"
#include "Operaciones.c"
#include <ctype.h>
#include <math.h>

GtkWidget* ventana=NULL;
GtkWidget* tabla_inicial=NULL,*tabla_oferta=NULL,*tabla_demanda=NULL;
Matriz*modelo=NULL;
int cantidad_oferta;

int isnumeric(char*str){
    int punto_decimal=0;
    while(*str){
        if(!isdigit(*str)){
            if((*str)==',' || (*str)=='.'){
                if(punto_decimal){
                    return 0;
                }else{
                    punto_decimal=1;
                }
            }else{
                return 0;
            }
        }
        str++;
    }
    return 1;
}


static void agrega_oferta(GtkWidget*widget,gpointer contenedor){
    GtkWidget*tab=(GtkWidget*)contenedor;
    if(tabla_oferta==NULL) tabla_oferta=tab;
    gtk_sheet_add_row((GtkSheet*)tabla_oferta,1);
}

static void cancel_mod_trans(GtkWidget*widget,gpointer acaba){
    GtkWidget*ventana=(GtkWidget*)acaba;
    modelo=NULL;
    tabla_oferta=NULL,tabla_demanda=NULL;
    gtk_widget_destroy(ventana);
}

int correcto(GtkSheet* tabla){
    int i;
    int cantidad=gtk_sheet_get_rows_count(GTK_SHEET(tabla));
    if(cantidad>0){
        for(i=0;i<cantidad;i++){
            char*str=gtk_sheet_cell_get_text(tabla,i,0);
            if(str){
                if(!isnumeric(str)){
                    return 0;
                }
            }else{
                return 0;
            }
        }
    }else{
        return 0;
    }
    return 1;
}

static void agrega_demanda(GtkWidget*widget,gpointer anterior){
    GtkWidget*tab=(GtkWidget*)anterior;
    if(tabla_demanda==NULL) tabla_demanda=tab;
    gtk_sheet_add_row((GtkSheet*)tabla_demanda,1);
}

static void destruye_dialogos(GtkWidget*widget,gpointer contenedor){
    gtk_widget_destroy((GtkWidget*)contenedor);
}

static void finalizar(GtkWidget*widget,gpointer anterior){
    if(correcto((GtkSheet*)tabla_demanda)){
        int i;
        int cantidad=gtk_sheet_get_rows_count(GTK_SHEET(tabla_demanda));
        for(i=0;i<cantidad;i++){
            double demanda=strtod(gtk_sheet_cell_get_text(GTK_SHEET(tabla_demanda),i,0),NULL);
            lista_add(&modelo->columnas,demanda);
        }
        gtk_widget_destroy((GtkWidget*)anterior);
        tabla_demanda=NULL;
        tabla_oferta=NULL;
        if(tabla_inicial!=NULL){
            gtk_widget_destroy(tabla_inicial);
            tabla_inicial=NULL;
        }
        int filas=get_size_lista(modelo->filas);
        int columnas=get_size_lista(modelo->columnas);
        tabla_inicial=gtk_sheet_new(filas+1,columnas+1,"Edit Table");
        gtk_box_pack_start(GTK_BOX(vbox),tabla_inicial,TRUE,TRUE,0);

        NodoLista*rec=modelo->columnas->inicio;
        for(i=0;i<columnas;i++){
            char c[20];
            sprintf(c,"%.2f",rec->valor);
            gtk_sheet_set_cell_text(GTK_SHEET(tabla_inicial),filas,i,c);
            rec=rec->sig;
        }
        rec=modelo->filas->inicio;
        for(i=0;i<filas;i++){
            char c[20];
            sprintf(c,"%.2f",rec->valor);
            gtk_sheet_set_cell_text(GTK_SHEET(tabla_inicial),i,columnas,c);
            rec=rec->sig;
        }
        gtk_sheet_row_button_add_label(GTK_SHEET(tabla_inicial),filas,"Demanda");
        gtk_sheet_column_button_add_label(GTK_SHEET(tabla_inicial),columnas,"Oferta");
        gtk_widget_show_all(vbox);

    }else{
        GtkWidget*aviso=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");

        GtkWidget*label=gtk_label_new("No es posible agregar ya que hay celdas vacías o no numéricas");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);

        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);

        gtk_widget_show_all(aviso);
    }
}

static void n_m_demanda(GtkWidget*widget,gpointer anterior){
    if(correcto((GtkSheet*)tabla_oferta)){
        int i;
        int cantidad=gtk_sheet_get_rows_count(GTK_SHEET(tabla_oferta));
        for(i=0;i<cantidad;i++){
            double oferta=strtod(gtk_sheet_cell_get_text(GTK_SHEET(tabla_oferta),i,0),NULL);
            lista_add(&modelo->filas,oferta);
        }

        gtk_widget_destroy((GtkWidget*)anterior);

        GtkWidget*ventana_nuevo=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(ventana_nuevo),"Nuevo Modelo: Demanda");
        gtk_widget_set_size_request(GTK_WIDGET(ventana_nuevo),450,400);

        GtkWidget*scrolled2=gtk_scrolled_window_new(NULL,NULL);
        gtk_container_set_border_width(GTK_CONTAINER(scrolled2),10);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled2),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),scrolled2,TRUE,TRUE,0);

        GtkWidget*sheet=gtk_sheet_new(0,1,"Edit Table");
        gtk_container_add(GTK_CONTAINER(scrolled2),sheet);

        GtkWidget*boton=gtk_button_new_with_label("Agregar Demanda");
        g_signal_connect(boton,"clicked",G_CALLBACK(agrega_demanda),sheet);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),boton,TRUE,TRUE,0);

        GtkWidget*sig=gtk_button_new_with_label("Finalizar");
        g_signal_connect(sig,"clicked",G_CALLBACK(finalizar),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),sig,TRUE,TRUE,0);

        GtkWidget*cancel=gtk_button_new_with_label("Cancelar");
        g_signal_connect(cancel,"clicked",G_CALLBACK(cancel_mod_trans),ventana_nuevo);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),cancel,TRUE,TRUE,0);

        gtk_widget_show_all(ventana_nuevo);
   }else{
        GtkWidget*aviso=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");

        GtkWidget*label=gtk_label_new("No es posible agregar ya que hay celdas vacías o no numéricas");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);

        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);

        gtk_widget_show_all(aviso);
    }
}

static void nuevo_mod_trans(GtkWidget*widget,gpointer data){
    modelo=(Matriz*)malloc(sizeof(Matriz));
    modelo->filas=0;
    modelo->columnas=0;

    GtkWidget*ventana_nuevo=gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(ventana_nuevo),"Nuevo Modelo: Oferta");
    gtk_widget_set_size_request(GTK_WIDGET(ventana_nuevo),400,400);

    GtkWidget*scrolled2=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled2),10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled2),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->vbox),scrolled2,TRUE,TRUE,0);

    GtkWidget*sheet=gtk_sheet_new(0,1,"Edit Table");
    gtk_container_add(GTK_CONTAINER(scrolled2),sheet);

    GtkWidget*boton=gtk_button_new_with_label("Agregar Oferta");
    g_signal_connect(boton,"clicked",G_CALLBACK(agrega_oferta),sheet);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),boton,TRUE,TRUE,0);

    GtkWidget*sig=gtk_button_new_with_label("Siguiente");
    g_signal_connect(sig,"clicked",G_CALLBACK(n_m_demanda),ventana_nuevo);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),sig,TRUE,TRUE,0);

    GtkWidget*cancel=gtk_button_new_with_label("Cancelar");
    g_signal_connect(cancel,"clicked",G_CALLBACK(cancel_mod_trans),ventana_nuevo);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_nuevo)->action_area),cancel,TRUE,TRUE,0);

    gtk_widget_show_all(ventana_nuevo);
}

static void esquina(GtkWidget*widget,gpointer pointer){
    if(tabla_inicial!=NULL){
        modelo=(Matriz*)malloc(sizeof(Matriz));
        modelo->filas=NULL;
        modelo->columnas=NULL;
        int filas=gtk_sheet_get_rows_count(GTK_SHEET(tabla_inicial));
        int columnas=gtk_sheet_get_columns_count(GTK_SHEET(tabla_inicial));
        int i;
        for(i=0;i<filas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,columnas-1);
            double demanda=strtod(str,NULL);
            addFila(&modelo,demanda);
        }
        for(i=0;i<columnas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),filas-1,i);
            double oferta=strtod(str,NULL);
            addColumna(&modelo,oferta);
        }

        for(i=0;i<filas-1;i++){
            int j;
            for(j=0;j<columnas-1;j++){
                char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,j);
                if(str){
                    if(isnumeric(str)){
                        double costo=strtod(str,NULL);
                        matriz_set_costo(&modelo,j,i,costo);
                    }else{
                        modelo=NULL;
                        GtkWidget*aviso=gtk_dialog_new();
                        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");
                        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);
                        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
                        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);
                        gtk_widget_show_all(aviso);
                        return;
                    }
                }else{
                    matriz_set_costo(&modelo,j,i,0);
                }
            }
        }
        esquina_noroeste(modelo);
    }else{
        GtkWidget*aviso=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");

        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);

        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);

        gtk_widget_show_all(aviso);
    }
}

static void vogel(GtkWidget*widget,gpointer pointer){
    if(tabla_inicial!=NULL){
        modelo=(Matriz*)malloc(sizeof(Matriz));
        modelo->filas=NULL;
        modelo->columnas=NULL;
        int filas=gtk_sheet_get_rows_count(GTK_SHEET(tabla_inicial));
        int columnas=gtk_sheet_get_columns_count(GTK_SHEET(tabla_inicial));
        int i;
        for(i=0;i<filas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,columnas-1);
            double demanda=strtod(str,NULL);
            addFila(&modelo,demanda);
        }
        for(i=0;i<columnas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),filas-1,i);
            double oferta=strtod(str,NULL);
            addColumna(&modelo,oferta);
        }

        for(i=0;i<filas-1;i++){
            int j;
            for(j=0;j<columnas-1;j++){
                char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,j);
                if(str){
                    if(isnumeric(str)){
                        double costo=strtod(str,NULL);
                        matriz_set_costo(&modelo,j,i,costo);
                    }else{
                        modelo=NULL;
                        GtkWidget*aviso=gtk_dialog_new();
                        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");
                        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);
                        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
                        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);
                        gtk_widget_show_all(aviso);
                        return;
                    }
                }else{
                    matriz_set_costo(&modelo,j,i,0);
                }
            }
        }
        aprox_vogel(modelo);
    }else{
        GtkWidget*aviso=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");

        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);

        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);

        gtk_widget_show_all(aviso);
    }
}

static void costos_minimos(GtkWidget*widget,gpointer pointer){
    if(tabla_inicial!=NULL){
        modelo=(Matriz*)malloc(sizeof(Matriz));
        modelo->filas=NULL;
        modelo->columnas=NULL;
        int filas=gtk_sheet_get_rows_count(GTK_SHEET(tabla_inicial));
        int columnas=gtk_sheet_get_columns_count(GTK_SHEET(tabla_inicial));
        int i;
        for(i=0;i<filas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,columnas-1);
            double demanda=strtod(str,NULL);
            addFila(&modelo,demanda);
        }
        for(i=0;i<columnas-1;i++){
            char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),filas-1,i);
            double oferta=strtod(str,NULL);
            addColumna(&modelo,oferta);
        }

        for(i=0;i<filas-1;i++){
            int j;
            for(j=0;j<columnas-1;j++){
                char*str=gtk_sheet_cell_get_text(GTK_SHEET(tabla_inicial),i,j);
                if(str){
                    if(isnumeric(str)){
                        double costo=strtod(str,NULL);
                        matriz_set_costo(&modelo,j,i,costo);
                    }else{
                        modelo=NULL;
                        GtkWidget*aviso=gtk_dialog_new();
                        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");
                        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);
                        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
                        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
                        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);
                        gtk_widget_show_all(aviso);
                        return;
                    }
                }else{
                    matriz_set_costo(&modelo,j,i,0);
                }
            }
        }
        costo_minimo(modelo,0);
    }else{
        GtkWidget*aviso=gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(aviso),"Error en la entrada");

        GtkWidget*label=gtk_label_new("No hay un modelo de transporte definido");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->vbox),label,TRUE,TRUE,0);

        GtkWidget*boton=gtk_button_new_with_label("Aceptar");
        g_signal_connect(boton,"clicked",G_CALLBACK(destruye_dialogos),aviso);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aviso)->action_area),boton,TRUE,TRUE,0);

        gtk_widget_show_all(aviso);
    }
}

static GtkItemFactoryEntry menu_items[] = {
  { "/Archivo",         NULL,         NULL,           0, "<Branch>" },
  { "/Archivo/_Nuevo",     "<control>N", NULL,    0, "<Branch>" },
  { "/Archivo/Nuevo/Modelo de _Transporte",     NULL, nuevo_mod_trans,    0, "<Item>" },
  { "/Archivo/sep1",     NULL,         NULL,           0, "<Separator>" },
  { "/Archivo/_Quit",    "<CTRL>Q", gtk_main_quit, 0, "<StockItem>", GTK_STOCK_QUIT },
  { "/_Resolver",     NULL, NULL,    0, "<Branch>" },
  { "/Resolver/_Esquina Noroeste",     "<CTRL>E", esquina,    0, "<Item>" },
  { "/Resolver/Costos _Mínimos",     "<CTRL>M", costos_minimos,    0, "<Item>" },
  { "/Resolver/Aproximación de _Vogel",     "<CTRL>G", vogel,    0, "<Item>" },
};

static gint nmenu_items=sizeof(menu_items)/sizeof(menu_items[0]);

static GtkWidget* get_barra_menu(GtkWidget* ventana){
    GtkItemFactory* itf;
    GtkAccelGroup* acg;

    acg=gtk_accel_group_new();

    itf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acg);

    gtk_item_factory_create_items(itf,nmenu_items,menu_items,NULL);

    gtk_window_add_accel_group(GTK_WINDOW(ventana),acg);

    return gtk_item_factory_get_widget(itf,"<main>");
}


int main (int argc, char *argv[])
{
    isnumeric("sd");
    cantidad_oferta=0;
    GtkWidget*barra;

    gtk_init(&argc,&argv);

    //creación de la ventana
    ventana=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(ventana,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(ventana),"Modelos de Transporte y Asignación");
    gtk_widget_set_size_request(GTK_WIDGET(ventana),1024,600);
    gtk_widget_set_uposition(GTK_WIDGET(ventana),250,100);
    //creando el vbox
    vbox=gtk_vbox_new(FALSE,1);
    gtk_container_set_border_width(GTK_CONTAINER(vbox),1);
    gtk_container_add(GTK_CONTAINER(ventana),vbox);

    //agregando el menu
    barra=get_barra_menu(ventana);

    //agregando al vbox
    gtk_box_pack_start(GTK_BOX(vbox),barra,FALSE,TRUE,0);

    //mostrando todo
    gtk_widget_show_all(ventana);

    gtk_main();
    return 0;
}
