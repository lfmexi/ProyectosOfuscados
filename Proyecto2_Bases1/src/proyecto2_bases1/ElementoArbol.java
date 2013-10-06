/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package proyecto2_bases1;

/**
 *
 * @author Luis Fernando
 */
public class ElementoArbol {
    
    private String icono;
    private String nombre;
    private String tipo;
    public ElementoArbol(String n,String t,String i){
        this.nombre=n;this.icono=i;
        tipo=t;
    }

    public String getIcono() {
        return icono;
    }

    public void setIcono(String icono) {
        this.icono = icono;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

}
