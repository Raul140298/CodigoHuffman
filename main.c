/* 
 * File:   COMPRESION DE HUFFMAN
 * Raul Jeri Lara       20156787
 * Juan Diego Veliz     20170089
 * Fabio Lopez          20155912
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX     256
#define MAXLIN  120


typedef struct charfrec{
    char c[MAX];
    int f;
    char codBIN[MAX*8];
}cf;

typedef struct NodoArbol{
    char valor[MAX];
    int frecuencia;
    struct NodoArbol*izq;
    struct NodoArbol*der;
    
}TNodoArbol;

typedef struct ArbolBinario{
    TNodoArbol*raiz;
}TArbolBinario;

void imprimeLinea(int n,char c){  
    for(int i=0;i<n;i++) printf("%c",c);
    printf("\n");   
}

FILE*abrirArchivo(const char*nombre,const char*modo){
    FILE*archivo;
    archivo=fopen(nombre,modo);
    if(archivo==NULL){
        printf("Error al intentar abrir el archivo:%s",nombre);
        exit(1);       
    }
    return archivo;
}

char*leerCadenaExacta(FILE*arch){
    char cadAux[500],*pt;
    fgets(cadAux,500,arch);
    pt=(char*)malloc(sizeof(char)*strlen(cadAux)+1);
    strcpy(pt,cadAux);
    return pt;
}

char* leerCadena(FILE*archivo){
    char *cadena;
    char* retorno,*aux;
    int l,i=0;
    

    while(1){
        
        cadena=leerCadenaExacta(archivo);
        if(i==0){
            retorno=cadena;
        }
        if(feof(archivo))break;
        l=strlen(cadena);
        if(i!=0){
            strcat(retorno,cadena);
        }
        
        i++;
        
    }

    return retorno;
}

int cuentaOcurrencias(char c,char* s){
    int i;
    for (i=0; s[i] ; s[i]==c ? i++ : *s++);
    return i;
}

int mayor(int x,int y){
    return(x>y?x:y);
}

void merge(cf* arr,int ini,int med,int fin){
    int l1,l2,i,p0=0,q0=0;
    
    l1=med-ini+1;
    l2=fin-med;
    
    cf p[l1+1];
    cf q[l2+1];
    
    for(i=ini;i<=med;i++){
        strcpy(p[i-ini].c,arr[i].c);
        p[i-ini].f=arr[i].f;
        
    }
    

    strcpy(p[l1].c,"$");
    p[l1].f=10000;
    
    for(i=med+1;i<=fin;i++){
        strcpy(q[i-med-1].c,arr[i].c);
        q[i-med-1].f=arr[i].f;
    }

    strcpy(q[l2].c,"@");
    q[l2].f=10000;
    
    for(i=ini;i<=fin;i++){
        if(p[p0].f<q[q0].f){
            strcpy(arr[i].c,p[p0].c);
            arr[i].f=p[p0++].f;           
        }
        else {
            strcpy(arr[i].c,q[q0].c);            
            arr[i].f=q[q0++].f;            
        }
    }   
}

void mergeSort(cf* arr,int ini,int fin){
    if(ini==fin)return;
    int med=(ini+fin)/2;
    
    mergeSort(arr,ini,med);
    mergeSort(arr,med+1,fin);
    merge(arr,ini,med,fin);
    
}

void generarBin(char* fuente,cf* chYfrec,char* codigo,int cantChar){
    
    for(int i=0;i<cantChar;i++){
        if(strcmp(fuente,chYfrec[i].c)==0){
            strcpy(chYfrec[i].codBIN,codigo);
            break;
        }
    }
       
}

void codigoBIN(TNodoArbol* nodo,cf* chYfrec,char* cadena,int cantChar){ 
    
    
    if(nodo->izq==NULL && nodo->der==NULL){        
        generarBin(nodo->valor,chYfrec,cadena,cantChar);       
        return;
    }
    
    char cadIzq[MAX],cadDer[MAX];
    strcpy(cadIzq,cadena);
    strcat(cadIzq,"0");
    strcpy(cadDer,cadena); 
    strcat(cadDer,"1");
    
    if (nodo) {
        codigoBIN(nodo->izq,chYfrec,cadIzq,cantChar);
        codigoBIN(nodo->der,chYfrec,cadDer,cantChar);
    }

}

int buscarCF(char dato,cf* chYfrec,int cantChar){
    
    for(int i=0;i<cantChar;i++)
        if(dato==chYfrec[i].c[0])
            return i;
  
}

void eliminarNodoArbol(TNodoArbol *nodo){
    if (nodo) {
        eliminarNodoArbol(nodo->izq);
        eliminarNodoArbol(nodo->der);
        free(nodo);
    }
}

void imprimeNodoArbol(TNodoArbol *nodo,int n,int k){
  
    if(nodo){     
    imprimeNodoArbol(nodo->izq,n,k+1);
    for (int i = 0; i <n*k; i++) printf("  ");
    printf("->(%s %d)\n", nodo->valor, nodo->frecuencia);
   
    imprimeNodoArbol(nodo->der,n,k+1);
    }
}

TNodoArbol* popArbol(TArbolBinario* ab,int*lonAB){

    TNodoArbol*aux=ab[0].raiz;
    
    for(int i=0;i<(*lonAB)-1;i++)
        ab[i]=ab[i+1];
    
    ab[*lonAB].raiz=NULL;
    
    (*lonAB)--;

    return aux;  
}

void aniadirOrdenado(TArbolBinario* ab,TNodoArbol* top,int*lonAB,int pos){
              
    for(int i=(*lonAB);i>pos;i--)
        ab[i]=ab[i-1]; 
    
    ab[pos].raiz=top;
    
    (*lonAB)++;
}

int busqueda(TArbolBinario* ab,int lonAB,int top){
    int i=0;
    
    while(top>ab[i].raiz->frecuencia){
        if(i==lonAB)break;
        i++;
    }
  
    return i;    
}

int cadBINaDEC(char *cadenaBinaria, int longitud) {
  int decimal = 0;
  int multiplicador = 1;
  char caracterActual;
  for (int i = longitud - 1; i >= 0; i--) {
    caracterActual = cadenaBinaria[i];
    if (caracterActual == '1') {
      decimal += multiplicador;
    }
    multiplicador = multiplicador * 2;
  }
  return decimal;
}

char* convertir(char* codificado){
    char* convertido;
    convertido=(char*)malloc(sizeof(char)*9);
    
    char temporal[9];
    for(int i=0;i<8;i++) temporal[i]='0';     
    temporal[8]=0;
    
    int l = strlen(codificado);

    
    for(int i=8-l,j=0; i<8, j<8; i++,j++){
        temporal[i]=codificado[j];     
    }
       
    strcpy(convertido,temporal);
 
    return convertido;
}

void crearArchivoBinario(char* dato,int l,cf* chYfrec,int cantChar){
    FILE*archivoBIN;
    archivoBIN=abrirArchivo("archivoBIN.bin","wb");
    
    //CREAMOS LA CABECERA DEL ARCHIVO BINARIO Y LA ALMACENAMOS
    int* entero=&cantChar;    
    fwrite(&entero,sizeof(int),1,archivoBIN); 
    
    //ALMACENAMOS CADA CARACTER Y FRECUENCIA
    for(int i=0;i<cantChar;i++){
        fwrite(&chYfrec[i].c,sizeof(char),1,archivoBIN);
        fwrite(&chYfrec[i].f,sizeof(int) ,1,archivoBIN);
    }
    
    int pos;
    char aux[l*8];
    strcpy(aux,"");
    
    for(int i=0;i<l;i++){
        pos=buscarCF(dato[i],chYfrec,cantChar);
        strcat(aux,chYfrec[pos].codBIN);
    }
  
    int laux=strlen(aux);
    int l2=laux;
        
    //ALMACENAMOS EL NUMERO DE BITS QUE TIENE EL TEXTO CODIFICADO
    fwrite(&laux,sizeof(int),1,archivoBIN); 
    
    printf("\nSu codigo generado es:\n");
    imprimeLinea(MAXLIN,'=');
    printf("%s\n",aux);
    imprimeLinea(MAXLIN,'=');
    printf("Su tamanio es: %d\n",laux);
        
    if(laux%8!=0){
        laux+=laux%8; 
        for(int i=l2;i<laux;i++)
            aux[i]='0';     
    }  
    
    
    int div8 = laux/8;
    char caracter[8];
    int carBIN;
    unsigned char final;
    int j=0;
    
    printf("\nCODIGO EN CARACTERES:\n");
    printf("(CARACTERES A ALMACENAR EN EL ARCHIVO BINARIO)\n");
    
    //ALMACENAMOS EL TEXTO CODIFICADO
    for(int i=0;i<div8;i++){
        
        for(int j=0;j<8;j++){
            caracter[j]=aux[i*8+j];
            printf("%c",caracter[j]);
        }
        
        carBIN=cadBINaDEC(caracter,8);
        printf(" -> %d\n",carBIN);
        
        final=(unsigned char)carBIN;
        fwrite(&final,sizeof(unsigned char),1,archivoBIN); 
        j=0;    
        
    }
    
    
    printf("\n");
    
    fclose(archivoBIN);
}

int buscarCF2(char bin[],cf chYfrec[], int cant){
    
    
    for(int j=0;j<cant;j++){
        if (strcmp(bin,chYfrec[j].codBIN)==0){
            return j;
        }
    }
    return -1;
}

void traducirBinario(char* cadena,cf* chYfrec,int numReg,int numReg2){
    
    char aux[numReg2+1];
    strcpy(aux,"");
    
    int pos=-1;
    
    for(int i=0;i<numReg2+1;i++){
    	strncat(aux,&cadena[i],1);
    	pos = buscarCF2(aux,chYfrec,numReg);
    	if (pos>=0){
    		printf("%c",chYfrec[pos].c[0]);
    		strcpy(aux,"");
    	}
    }      
    
}

void leerArchivoBinario(){   
    FILE*archivoBIN;
    archivoBIN=abrirArchivo("archivoBIN.bin","rb");
    
    //LEE EL NUMERO DE CARACTERES
    printf("DISPOSICION DE LA CABECERA:\n");
    int* numReg;
    fread(&numReg,sizeof(int),1,archivoBIN);
    printf("[%d]",*numReg);   

    //LEEMOS CADA CARACTER CON SU FRECUENCIA
    int  *ptUnc;
    char *ptChar;     
    
    cf chYfrecBIN[MAX];   

    //CREAMOS LA ESTRUCTURA CHARyFREC
    for(int i=0;i<*numReg;i++){ 
        
        //LEEMOS EL CARACTER Y LO GUARDAMOS
        fread(&ptChar,sizeof(char),1,archivoBIN);
        chYfrecBIN[i].c[0]=ptChar;
        chYfrecBIN[i].c[1]='\0';
 
        printf("[%s]",chYfrecBIN[i].c);   
         
        //LEEMOS LA FRECUENCIA Y LA GUARDAMOS
        fread(&ptUnc,sizeof(int),1,archivoBIN);
        chYfrecBIN[i].f=(int)ptUnc;
        printf("[%d]",chYfrecBIN[i].f);
    }
    
    //CREAMOS EL ARREGLO DE RAICES ARBOL
    int lonAB=*numReg;
    
    TArbolBinario ab[lonAB];
    
    for(int i=0;i<lonAB;i++){
        ab[i].raiz=(TNodoArbol*)malloc(sizeof(TNodoArbol));
        strcpy(ab[i].raiz->valor,chYfrecBIN[i].c);
        ab[i].raiz->frecuencia=chYfrecBIN[i].f;
        ab[i].raiz->izq=NULL;
        ab[i].raiz->der=NULL;   
    }
    
    TNodoArbol* izquierda;
    TNodoArbol* derecha;
    TNodoArbol* top;
    
    printf("\nCREACION DE LA ESTRUCTURA:\n");
    printf("\nCaracter   Frecuencia\n");   
    
    for(int i=0;i<lonAB;i++){
        printf("%4s  %10d\n",ab[i].raiz->valor,ab[i].raiz->frecuencia);               
    }
    
    int posicion;
    
    while(1){
    
        izquierda = popArbol(ab,&lonAB);
        
        derecha   = popArbol(ab,&lonAB);
        
        top=(TNodoArbol*)malloc(sizeof(TNodoArbol));
        
        strcpy(top->valor,izquierda->valor);
        strcat(top->valor,derecha->valor);
        top->frecuencia=izquierda->frecuencia+derecha->frecuencia;        
        top->izq=izquierda;
        top->der=derecha;
        
        posicion = busqueda(ab,lonAB,top->frecuencia);
                   
        aniadirOrdenado(ab,top,&lonAB,posicion);       

        if(lonAB==1)break;

    }
    
    printf("\nEL ARBOL BINARIO ES:\n\n");
    imprimeNodoArbol(ab[0].raiz,*numReg,0);
    printf("\n");
    
    codigoBIN(ab[0].raiz,chYfrecBIN,"",*numReg);
    
    printf("Caracter   Codigo Binario\n"); 
    for(int i=0;i<*numReg;i++){
        printf("%4s           %6s\n",chYfrecBIN[i].c,chYfrecBIN[i].codBIN);               
    }
    
    eliminarNodoArbol(ab[0].raiz);

    
    
    //LEEMOS EL NUMERO DE BITS CODIFICADOS Y DESCARTAMOS LA SOBRA
    printf("NUMERO DE BITS QUE DEBEN SER LEIDOS DEL ARCHIVO BINARIO:\n");
    int* numReg2;
    fread(&numReg2,sizeof(int),1,archivoBIN);
    printf("[%d]",numReg2); 
    
    
    //PROCEDEMOS A IMPRIMIR EL TEXTO
    unsigned char muestra;
    char buffer[9];
    char codigo[9];
    int numDeco = (int)numReg2;
    numDeco+=8;
    
    char decodificado[numDeco];
    numDeco-=8;
    char final[numDeco];
    
    strcpy(decodificado,"");
    
    printf("\nTEXTO CODIFICADO:\n");  
    while(1){
        fread(&muestra,sizeof(unsigned char),1,archivoBIN);
        if(feof(archivoBIN))break;
             
        strcpy(codigo,convertir(itoa(muestra,buffer,2)) );
        
        strcat(decodificado,codigo);
    }       
    strncpy(final,decodificado,numDeco);
    printf("%s\n\n",final);
    
    
    int a=(int)*numReg;
    int b=(int)numReg2;
    
    
    //DECODIFICACION DEL TEXTO CODIFICADO E IMPRESION EN PANTALLA    
    printf("SU ARCHIVO FINAL ES:\n");
    imprimeLinea(MAXLIN,'=');
    traducirBinario(final,chYfrecBIN,a,b);
    printf("\n");
    imprimeLinea(MAXLIN,'=');
    printf("\n");
    
    
    fclose(archivoBIN);
}

int main(int argc, char** argv) {
    printf("CODIFICACION HUFFMAN\n");
    
    FILE*archivo;
    archivo=abrirArchivo("archivo.txt","r");
    
    char* dato;
    dato=leerCadena(archivo);
    int tam = strlen(dato); 
    
    printf("\nSu mensaje es:\n");
    imprimeLinea(MAXLIN,'=');
    printf("%s",dato);
    printf("\n");
    imprimeLinea(MAXLIN,'=');
    printf("Su tamanio es: %d\n\n",tam*8);

    
    cf chYfrec[MAX];
    char solochar[MAX]="";

    char *pch;   
    int cantChar=0;
    char aux[2];
    int frec;

    for(int i=0;i<tam;i++){
        pch = (char*)memchr(solochar,dato[i],strlen(solochar));

        if (pch==NULL){
            frec = cuentaOcurrencias(dato[i],dato);
            
            aux[0]=dato[i];
            aux[1]='\0';

            strcpy(chYfrec[cantChar].c,aux);
            chYfrec[cantChar].f=frec;

            solochar[cantChar]=dato[i];
            cantChar++;
        }
    }

    mergeSort(chYfrec,0,cantChar-1);
 
    for(int i=0;i<cantChar;i++)
        solochar[i]=chYfrec[i].c[0];
    
    
    int lonAB=cantChar;
    
    TArbolBinario ab[lonAB];
    
    for(int i=0;i<lonAB;i++){
        ab[i].raiz=(TNodoArbol*)malloc(sizeof(TNodoArbol));
        strcpy(ab[i].raiz->valor,chYfrec[i].c);
        ab[i].raiz->frecuencia=chYfrec[i].f;
        ab[i].raiz->izq=NULL;
        ab[i].raiz->der=NULL;   
    }
    
    TNodoArbol* izquierda;
    TNodoArbol* derecha;
    TNodoArbol* top;
    
    printf("Caracter   Frecuencia\n");   
    
    for(int i=0;i<lonAB;i++){
        printf("%4s  %10d\n",ab[i].raiz->valor,ab[i].raiz->frecuencia);               
    }
    
    int posicion;
    
    while(1){
    
        izquierda = popArbol(ab,&lonAB);
        
        derecha   = popArbol(ab,&lonAB);
        
        top=(TNodoArbol*)malloc(sizeof(TNodoArbol));
        
        strcpy(top->valor,izquierda->valor);
        strcat(top->valor,derecha->valor);
        top->frecuencia=izquierda->frecuencia+derecha->frecuencia;        
        top->izq=izquierda;
        top->der=derecha;
        
        posicion = busqueda(ab,lonAB,top->frecuencia);
                   
        aniadirOrdenado(ab,top,&lonAB,posicion);       

        if(lonAB==1)break;

    }
    
    
    printf("\nEL ARBOL BINARIO ES:\n\n");
    imprimeNodoArbol(ab[0].raiz,cantChar,0);
    printf("\n");
    
    codigoBIN(ab[0].raiz,chYfrec,"",cantChar);
    
    printf("Caracter   Codigo Binario\n"); 
    for(int i=0;i<cantChar;i++){
        printf("%4s           %6s\n",chYfrec[i].c,chYfrec[i].codBIN);               
    }
    
    eliminarNodoArbol(ab[0].raiz);
    
    
    crearArchivoBinario(dato,tam,chYfrec,cantChar);
    
    
    printf("ARCHIVO BINARIO:\n");
    
    leerArchivoBinario();
    
    printf("POR FAVOR, AL FINALIZAR, VERIFIQUE QUE, EFECTIVAMENTE, EL ARCHIVO BIN PESA MENOS QUE EL .TXT\n\n");
    
    return (EXIT_SUCCESS);
}
//bitwise operator in C

