#include "string"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <time.h>
#include "../estructuras/estructuras.hpp"

using namespace std;

Particion crearParticion(int size, string name, char tipo, char fit, int inicio)
{
  Particion newParticion;
  newParticion.part_fit = fit;
  newParticion.part_name = name;
  newParticion.part_size = size;
  newParticion.part_start = inicio;
  newParticion.part_status = 'c'; //conectada
  newParticion.part_type = tipo;
  return newParticion;
}
int espacioDisponible(MBR mbr)
{
  for (int i = 0; i < 4; i++)
  {
    if (mbr.mbr_partition[i].part_size == 0)
    {
      return i;
    }
    cout << "Size: " << mbr.mbr_partition[i].part_size << endl;
  }

  return -1;
}
Particion *burbuja(Particion a[4])
{
  int i, j;
  Particion temp;
  for (i = 1; i < 4; i++)
    for (j = 4 - 1; j >= i; j--)
      if (a[j - 1].part_start > a[j].part_start)
      {
        temp = a[j - 1];
        a[j - 1] = a[j];
        a[j] = temp;
      }
  return a;
}
int getInicioParticion(MBR mbr, float size_partition)
{
  Particion *particiones = burbuja(mbr.mbr_partition);
  int inicio = sizeof(mbr);
  for (int i = 0; i < 3; i++)
  {

    if (particiones[i].part_size == 0)
    {
      return inicio; //Primera posicion vacia
    }
    else if (particiones[i + 1].part_size == 0)
    {
      return particiones[i].part_start + particiones[i].part_size; //Si el siguiente esta vacio
    }
    else if (particiones[i].part_start + particiones[i].part_size - particiones[i + 1].part_start >= size_partition)
    {
      return particiones[i].part_start + particiones[i].part_size; //Si cabe entre esos dos
    }
  }

  return -1;
}
void escribirMBR(MBR mbr, char *ruta)
{
  FILE *file = NULL;
  file = fopen(ruta, "r");

  cout << "Escribiendo MBR...\nFecha de creacion: " << asctime(gmtime(&mbr.mbr_fecha_creacion)) << endl;
  fseek(file, 0, SEEK_SET);
  fwrite(&mbr, sizeof(MBR), 1, file);

  fclose(file);
}

vector<string> splits(string str, char pattern)
{

  int posInit = 0;
  int posFound = 0;
  string splitted;
  vector<string> results;
  while (posFound >= 0)
  {
    posFound = str.find(pattern, posInit);

    splitted = str.substr(posInit, posFound - posInit);
    posInit = posFound + 1;
    results.push_back(splitted);
  }

  return results;
}

void mkdir(string path)
{
  vector<string> carpetas = splits(path, '/');
  if (carpetas.size() > 1)
  {
    string ruta = "mkdir -p ";
    for (int i = 0; i < carpetas.size() - 1; i++)
    {
      ruta += carpetas[i] + "/";
    }
    char r[ruta.size()];
    strcpy(r, ruta.c_str());
    system(r);
  }
}

void mkdisk(string path, int size, char ajuste, char unidad)
{
  cout << "Creación de disco exitosa!!" << endl;
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());
  mkdir(path);
  FILE *file = NULL;
  file = fopen(ruta, "r");
  if (file != NULL)
  {
    cout << "ERROR: El archivo ya existe dentro de la ruta actual" << endl;
    return;
  }
  if (unidad == 'k')
  {
    size = size * 1024;
  }
  else
  {
    size = size * 1024 * 1024;
  }
  file = fopen(ruta, "wb");
  fwrite("\0", 1, 1, file);
  fseek(file, size, SEEK_SET);
  fwrite("\0", 1, 1, file);

  MBR mbr;
  mbr.mbr_tamano = size;
  mbr.mbr_disk_signature = rand() % 1000;
  mbr.mbr_fecha_creacion = time(0);
  for (int i = 0; i < 4; i++)
  {
    mbr.mbr_partition[i].part_status = '0';
    mbr.mbr_partition[i].part_size = 0;
    mbr.mbr_partition[i].part_fit = 'f';
    mbr.mbr_partition[i].part_start = size;
    mbr.mbr_partition[i].part_name = "";
  }
  cout << "Escribiendo MBR...\nFecha de creacion: " << asctime(gmtime(&mbr.mbr_fecha_creacion)) << endl;
  fseek(file, 0, SEEK_SET);
  fwrite(&mbr, sizeof(MBR), 1, file);

  fclose(file);
}
MBR getMBR(string path)
{
  MBR mbr;
  //Depurar ruta
 
  char ruta[path.size()];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());
  FILE *file = NULL;
  cout << "#" << ruta << "#" << endl;
  if ((file = fopen(ruta, "rb+")))
  {
    size_t res;
    fseek(file, 0, SEEK_SET);
    res = fread(&mbr, sizeof(mbr) , 1, file);

    if (res != 1)
    {
      perror("Error al leer el archivo ");
    }
  }
  else
  {
    perror("Error al abrir archivo ");
  }

  if (fclose(file) != 0)
  {
    perror("Error al cerrar: ");
  }
  return mbr;
}
//size, u,string,type,f,name
void fdisk(float size, char unidad, string path, char type, char fit, string name)
{
  MBR mbr;
   mbr.mbr_tamano = 0;
  mbr.mbr_disk_signature = rand() % 1000;
  mbr.mbr_fecha_creacion = time(0);
  for (int i = 0; i < 4; i++)
  {
    mbr.mbr_partition[i].part_status = '0';
    mbr.mbr_partition[i].part_size = 0;
    mbr.mbr_partition[i].part_fit = 'f';
    mbr.mbr_partition[i].part_start = 0;
    mbr.mbr_partition[i].part_name = "";
  }
  mbr=getMBR(path);
  cout << "Size del mbr obtenido: " << mbr.mbr_tamano << endl;

  //cout<<mbr.mbr_tamano<<endl;
  /*if (mbr.mbr_tamano > 0)
  {
    int e_disponible = espacioDisponible(mbr);
    if (e_disponible != -1)
    {
      int inicio = getInicioParticion(mbr, size);
      if (inicio!=-1)
      {
        Particion nuevaParticion=crearParticion( size,  name,  type,  fit,  inicio);
        mbr.mbr_partition[e_disponible]=nuevaParticion;
        char ruta[path.size() + 1];
        path.replace(path.size() - 1, 1, "");
        strcpy(ruta, path.c_str());
        escribirMBR(mbr,ruta);
      }
      else
      {
        cout<<"ERROR: No hay espacio para la partición "<<name<<endl;
      }
      
    }
    else
    {
      cout << "Error: No se pueden hacer más particiones" << endl;
    }
  }else{
    cout<<"ERROR: Error al obtener el mbr"<<endl;
  }*/
}

//fdisk -Size=300 -path=discos/disco3.disk -name=Particion1
