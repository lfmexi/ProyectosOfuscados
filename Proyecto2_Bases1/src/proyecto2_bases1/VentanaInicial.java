/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package proyecto2_bases1;

/**
 *
 * @author Luis Fernando
 */
import java.sql.*;
import java.util.LinkedList;
import javax.swing.table.DefaultTableModel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
public class VentanaInicial extends javax.swing.JFrame {

    /**
     * Creates new form VentanaInicial
     */
    String host;
    String user;
    String pass;
    private String db;
    private String tabla;
    private LinkedList<String> columnas;
    private LinkedList<String> nullables;
    private LinkedList<String> tipos;
    private LinkedList<String> pks;
    private LinkedList<String> fks;
    
    public VentanaInicial() {
        initComponents();
        pass="";host="";user="";
        DefaultTreeModel modelo=new DefaultTreeModel(null);
        arbolBases.setModel(modelo);
    }

    
    private boolean forma1FN(){
        if(tabla!=null && db!=null){
            if(!tabla.equals("") && !db.equals("")){
                if(this.pks.size()>0){
                    String cs="jdbc:postgresql://"+this.host+"/"+db+"?user="+this.user
                        +"&password="+this.pass;
                    try{
                        Class.forName("org.postgresql.Driver");
                        Connection conn=DriverManager.getConnection(cs);
                        Statement st=conn.createStatement();
                        String sql="select distinct ";
                        for(int i=0;i<pks.size();i++){
                            if(i<pks.size()-1){
                                sql=sql+pks.get(i)+",";
                            }else{
                                sql=sql+pks.get(i);
                            }
                        }
                        sql=sql+" from "+tabla;
                        ResultSet res=st.executeQuery(sql);
                        int contadorPks=0;
                        while(res.next()){
                            contadorPks++;
                        }
                        st=conn.createStatement();
                        sql="select count(*) as conteo from "+tabla;
                        res=st.executeQuery(sql);
                        int contadorTabla=0;
                        if(res.next()){
                            contadorTabla=Integer.parseInt(res.getString("conteo"));
                        }
                        if(contadorTabla==contadorPks){
                            for(int i=0;i<columnas.size();i++){
                                if(pks.contains(columnas.get(i))){
                                    if(nullables.get(i).equals("YES")){
                                        conn.close();
                                        return false;
                                    }
                                }
                                st=conn.createStatement();
                                sql="select "+columnas.get(i)+" from "+tabla;
                                res=st.executeQuery(sql);
                                while(res.next()){
                                    String str=res.getString(columnas.get(i));
                                    if(str.contains("|")||str.contains(";") || str.contains("//")){
                                        conn.close();
                                        return false;
                                    }
                                }
                            }
                            conn.close();
                            return true;
                        }
                        conn.close();
                    }catch(Exception e){System.out.println(e);}
                }
            }
        }
        return false;
    }
    
    
    private boolean forma2FN(){
        if(tabla!=null && db!=null){
            if(!tabla.equals("") && !db.equals("")){
                if(pks.size()<=1){
                    return true;
                }else{
                    String cs="jdbc:postgresql://"+this.host+"/"+db+"?user="+this.user
                        +"&password="+this.pass;
                    try{
                        Class.forName("org.postgresql.Driver");
                        Connection conn=DriverManager.getConnection(cs);
                        for(int i=0;i<pks.size();i++){
                            for(int j=0;j<columnas.size();j++){
                                if(!pks.contains(columnas.get(j)) && !fks.contains(pks.get(i))){
                                    String sql;
                                    Statement st=conn.createStatement();
                                    sql="select "+pks.get(i)+","+columnas.get(j)+", count(*) as conteo from "+tabla
                                            + " group by "+pks.get(i)+","+columnas.get(j);

                                    ResultSet res2=st.executeQuery(sql);
                                    while(res2.next()){
                                        int cantidad=Integer.parseInt(res2.getString("conteo"));
                                        if(cantidad>1){
                                            String str=res2.getString(pks.get(i));
                                            if(str.contains("\'")){str=str.replace("\'", "\'\'");}
                                            sql="select count(*) ct from "+tabla+" where "+pks.get(i)+" = \'"+str+"\'";
                                            st=conn.createStatement();
                                            ResultSet rs=st.executeQuery(sql);
                                            if(rs.next()){
                                                int cant=Integer.parseInt(rs.getString("ct"));
                                                if(cant==cantidad){
                                                    
                                                        str=res2.getString(columnas.get(j));
                                                        if(str.contains("\'")){str=str.replace("\'", "\'\'");}
                                                        st=conn.createStatement();
                                                        sql="select count(*) ct2 from "+tabla+" where "+columnas.get(j)+"=\'"+str+"\'";
                                                        ResultSet rs2=st.executeQuery(sql);
                                                        if(rs2.next()){
                                                            int cant2=Integer.parseInt(rs2.getString("ct2"));
                                                            if(cant2==cantidad){
                                                                conn.close();
                                                                return false;
                                                            }
                                                        }
                                                    
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        conn.close();
                        return true;
                    }catch(Exception e){System.out.println(e);}
                }
            }
        }
        return false;
    }
    
    
    private boolean forma3FN(){
        if(tabla!=null && db!=null){
            if(!tabla.equals("") && !db.equals("")){
                String cs="jdbc:postgresql://"+this.host+"/"+db+"?user="+this.user
                        +"&password="+this.pass;
                try{
                    Class.forName("org.postgresql.Driver");
                    Connection conn=DriverManager.getConnection(cs);
                    for(int i=0;i<columnas.size();i++){
                        for(int j=0;j<columnas.size();j++){
                            if(i!=j && !pks.contains(columnas.get(i))&&!pks.contains(columnas.get(j))){
                                String sql;
                                Statement st=conn.createStatement();
                                sql="select "+columnas.get(i)+","+columnas.get(j)+",count(*) cantidad from "+tabla
                                        + " group by "+columnas.get(i)+","+columnas.get(j);
                                ResultSet res2=st.executeQuery(sql);
                                while(res2.next()){
                                    int cantidad=Integer.parseInt(res2.getString("cantidad"));
                                    if(cantidad>1){
                                        String str=res2.getString(columnas.get(i));
                                        if(str.contains("\'")){str=str.replace("\'", "\'\'");}
                                        st=conn.createStatement();
                                        sql="select count(*) ct from "+tabla+
                                                " where "+columnas.get(i)+" = \'"+str+"\' ";
                                        ResultSet rs=st.executeQuery(sql);
                                        if(rs.next()){
                                            int cant=Integer.parseInt(rs.getString("ct"));
                                            if(cant==cantidad){
                                                    str=res2.getString(columnas.get(j));
                                                    if(str.contains("\'")){str=str.replace("\'", "\'\'");}
                                                    st=conn.createStatement();
                                                    sql="select count(*) ct2 from "+tabla+" where "+columnas.get(j)+"=\'"+str+"\'";
                                                    ResultSet rs2=st.executeQuery(sql);
                                                    if(rs2.next()){
                                                        int cant2=Integer.parseInt(rs2.getString("ct2"));
                                                        if(cant2==cantidad){
                                                            conn.close();
                                                            return false;
                                                        }
                                                    }
                                                
                                            }
                                            
                                        }
                                    }
                                }
                            }
                        }
                    }
                    conn.close();
                    return true;
                }catch(Exception e){System.out.println(e);}
            }
        }
        return false;
    }
    
    private boolean FNCB(){
        if(tabla!=null && db!=null){
            if(!tabla.equals("") && !db.equals("")){
                if(pks.size()<=1){
                    return true;
                }else{
                    return false;
                }
            }
        }
        return false;
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        boton1 = new javax.swing.JButton();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jScrollPane1 = new javax.swing.JScrollPane();
        arbolBases = new javax.swing.JTree();
        jTabbedPane2 = new javax.swing.JTabbedPane();
        jScrollPane2 = new javax.swing.JScrollPane();
        tablaData = new javax.swing.JTable();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane3 = new javax.swing.JScrollPane();
        tablaInfo = new javax.swing.JTable();
        botonDetermina = new javax.swing.JButton();
        labelMuestra = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Normalizador de BD");

        boton1.setText("Nueva Conexión");
        boton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                boton1ActionPerformed(evt);
            }
        });

        arbolBases.addTreeSelectionListener(new javax.swing.event.TreeSelectionListener() {
            public void valueChanged(javax.swing.event.TreeSelectionEvent evt) {
                arbolBasesValueChanged(evt);
            }
        });
        jScrollPane1.setViewportView(arbolBases);

        jTabbedPane1.addTab("Bases de Datos", jScrollPane1);

        tablaData.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {

            }
        ));
        jScrollPane2.setViewportView(tablaData);

        jTabbedPane2.addTab("Data", jScrollPane2);

        tablaInfo.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Columna", "Tipo", "Nullable", "PK", "FK"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane3.setViewportView(tablaInfo);

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 499, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 357, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane3, javax.swing.GroupLayout.DEFAULT_SIZE, 402, Short.MAX_VALUE)
        );

        jTabbedPane2.addTab("Columnas", jPanel1);

        botonDetermina.setText("Determinar Forma");
        botonDetermina.setEnabled(false);
        botonDetermina.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                botonDeterminaActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(18, 18, 18)
                                .addComponent(jTabbedPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 861, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(boton1)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(434, 434, 434)
                        .addComponent(botonDetermina)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(labelMuestra, javax.swing.GroupLayout.PREFERRED_SIZE, 434, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(158, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(boton1)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jTabbedPane2)
                    .addComponent(jTabbedPane1))
                .addGap(18, 18, 18)
                .addComponent(botonDetermina)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(labelMuestra, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(147, 147, 147))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    
    private void boton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_boton1ActionPerformed
        // TODO add your handling code here:
        Ventana_Nueva_Conexion v=new Ventana_Nueva_Conexion(this);
        v.setVisible(true);
    }//GEN-LAST:event_boton1ActionPerformed

    private void arbolBasesValueChanged(javax.swing.event.TreeSelectionEvent evt) {//GEN-FIRST:event_arbolBasesValueChanged
        // TODO add your handling code here:
        DefaultMutableTreeNode nodo=(DefaultMutableTreeNode) arbolBases.getLastSelectedPathComponent();
        if(nodo==null) return;
        if(nodo.isLeaf()&& ((ElementoArbol)nodo.getUserObject()).getTipo().equals("db")){
            String dbname=((ElementoArbol)nodo.getUserObject()).getNombre();
            this.db=dbname;
            String cs="jdbc:postgresql://"+this.host+"/"+dbname+"?user="+this.user
                        +"&password="+this.pass;
            try{
                Class.forName("org.postgresql.Driver");
                Connection conn=DriverManager.getConnection(cs);
                Statement st=conn.createStatement();
                String sql="select tablename from pg_tables where schemaname=\'public\'";
                ResultSet res=st.executeQuery(sql);
                while(res.next()){
                    DefaultMutableTreeNode nuevo=new DefaultMutableTreeNode(new ElementoArbol(res.getString("tablename"),"tab","/proyecto2_bases1/table.png"));
                    nodo.add(nuevo);
                }
                arbolBases.setCellRenderer(new elemntoCellReader());
                arbolBases.repaint();
                conn.close();
            }catch(Exception e){System.out.println(e);}
        }else if(nodo.isLeaf() && ((ElementoArbol)nodo.getUserObject()).getTipo().equals("tab")){
            this.columnas=new LinkedList<String>();
            this.tipos=new LinkedList<String>();
            this.nullables=new LinkedList<String>();
            this.pks=new LinkedList<String>();
            this.fks=new LinkedList<String>();
            String dbname=((ElementoArbol)((DefaultMutableTreeNode)nodo.getParent()).getUserObject()).getNombre();
            String tablename=((ElementoArbol)nodo.getUserObject()).getNombre();
            this.tabla=tablename;
            this.db=dbname;
            String cs="jdbc:postgresql://"+this.host+"/"+dbname+"?user="+this.user
                        +"&password="+this.pass;
            try{
                Class.forName("org.postgresql.Driver");
                Connection conn=DriverManager.getConnection(cs);
                Statement st=conn.createStatement();
                String sql="SELECT c.column_name AS columna, c.data_type as tipo, "
                        + "c.is_nullable AS nullable "
                        + "FROM information_schema.columns c "
                        + "LEFT JOIN information_schema.element_types e ON "
                        + "c.table_catalog = e.object_catalog AND "
                        + "c.table_schema = e.object_schema AND "
                        + "c.table_name = e.object_name AND "
                        + "\'TABLE\'= e.object_type "
                        + "WHERE UPPER(c.table_name) = upper( \'"+tablename+"\' ) "
                        + "ORDER BY c.ordinal_position";
                ResultSet res=st.executeQuery(sql);
                ModeloTabla modelo=new ModeloTabla();
                while(res.next()){
                    columnas.add(res.getString("columna"));
                    nullables.add(res.getString("nullable"));
                    tipos.add(res.getString("tipo"));
                    String strCol=res.getString("columna")+" "+res.getString("tipo");
                    modelo.addColumn(strCol);
                }
                st=conn.createStatement();
                sql="select";
                for(int i=0;i<columnas.size();i++){
                    if(i<columnas.size()-1){
                        sql=sql+" "+columnas.get(i)+",";
                    }else{
                        sql=sql+" "+columnas.get(i);
                    }
                }
                sql=sql+" from "+tablename;
                res=st.executeQuery(sql);
                while(res.next()){
                    String [] reg=new String[columnas.size()];
                    for(int i=0;i<reg.length;i++){
                        reg[i]=res.getString(columnas.get(i));
                    }
                    modelo.addRow(reg);
                }
                this.tablaData.setModel(modelo);
                st=conn.createStatement();
                sql="select column_name as columna from information_schema.key_column_usage "
                        + "where upper(table_name)=upper(\'"+tablename+"\') and "
                        + "constraint_name like \'%_pkey%\'";
                res=st.executeQuery(sql);
                while(res.next()){
                    pks.add(res.getString("columna"));
                }
                st=conn.createStatement();
                sql="select column_name as columna from information_schema.key_column_usage "
                        + "where upper(table_name)=upper(\'"+tablename+"\') and "
                        + "constraint_name like \'%_fkey%\'";
                res=st.executeQuery(sql);
                while(res.next()){
                    fks.add(res.getString("columna"));
                }
                modelo=new ModeloTabla();
                modelo.addColumn("Columna");
                modelo.addColumn("Tipo");
                modelo.addColumn("Nullable");
                modelo.addColumn("PK");
                modelo.addColumn("FK");
                
                int cantidadCol=columnas.size();
                for(int i=0;i<cantidadCol;i++){
                    String [] arreglo=new String[5];
                    arreglo[0]=columnas.get(i);
                    arreglo[1]=tipos.get(i);
                    arreglo[2]=nullables.get(i);
                    if(pks.contains(columnas.get(i)))
                        arreglo[3]="YES";
                    else
                        arreglo[3]="NO";
                    if(fks.contains(columnas.get(i)))
                        arreglo[4]="YES";
                    else
                        arreglo[4]="NO";
                    modelo.addRow(arreglo);
                }
                this.tablaInfo.setModel(modelo);
                conn.close();
                botonDetermina.setEnabled(true);
                labelMuestra.setText("");
            }catch(Exception e){System.out.println(e);}
        }
    }//GEN-LAST:event_arbolBasesValueChanged

    private void botonDeterminaActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_botonDeterminaActionPerformed
        // TODO add your handling code here:
        if(forma1FN()){
            if(forma2FN()){
                if(forma3FN()){
                    if(FNCB()){
                        labelMuestra.setText("La tabla "+tabla+" se encuentra en FNCB");
                    }else
                        labelMuestra.setText("La tabla "+tabla+" se encuentra en 3FN");
                }else{
                    labelMuestra.setText("La tabla "+tabla+" se encuentra en 2FN");
                }
            }else
                labelMuestra.setText("La tabla "+tabla+" se encuentra en 1FN");
        }else{
            labelMuestra.setText("La tabla "+tabla+" no se encuentra en ninguna forma normal");
        }
    }//GEN-LAST:event_botonDeterminaActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(VentanaInicial.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(VentanaInicial.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(VentanaInicial.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(VentanaInicial.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new VentanaInicial().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    javax.swing.JTree arbolBases;
    private javax.swing.JButton boton1;
    private javax.swing.JButton botonDetermina;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JTabbedPane jTabbedPane2;
    private javax.swing.JLabel labelMuestra;
    private javax.swing.JTable tablaData;
    private javax.swing.JTable tablaInfo;
    // End of variables declaration//GEN-END:variables
}

class ModeloTabla extends DefaultTableModel{
    @Override
    public boolean isCellEditable(int row,int column){
        return false;
    }
}