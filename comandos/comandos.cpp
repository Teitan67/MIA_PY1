#include "string"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <time.h>
#include "../estructuras/estructuras.hpp"

using namespace std;

void mkdisk(string path,int size, char ajuste,char unidad){
    cout<<"Creación de disco exitosa!!"<<endl;
    char ruta[path.size() + 1];
    strcpy(ruta, path.c_str());
    FILE *file=NULL;
    file=fopen(ruta,"r");
    if(file!=NULL){
        cout<<"ERROR: El archivo ya existe dentro de la ruta actual"<<endl;
        return;
    }
    cout<<"Salida 1: "<<size<< endl;
    if(unidad=='k'){
      size =  size* 1024;
    }
    else{ 
      size =  size* 1024 * 1024;
    }
    cout<<"Salida: "<<size<< endl;
    file=fopen(ruta,"wb");
    fwrite("\0",1,1,file);
    fseek(file,size,SEEK_SET);
    fwrite("\0",1,1,file);

    MBR mbr;
    mbr.mbr_tamano = size;
    mbr.mbr_disk_signature = rand()%1000;
    mbr.mbr_fecha_creacion = time(0);
    for(int i = 0; i < 4; i++){
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_fit = 'f';
        mbr.mbr_partition[i].part_start = size;
        strcpy(mbr.mbr_partition[i].part_name,"");
    }
    cout<<"Escribiendo MBR...\nFecha de creacion: "<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;
    fseek(file,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1,file);

    fclose(file);
}

