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

bool nombreDisponible(Particion partciones[4], string name)
{
  for (size_t i = 0; i < 4; i++)
  {
    if (partciones[i].part_name == name)
    {
      return false;
    }
  }
  return true;
}

void escribirMBR(MBR mbr, string path)
{
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());
  FILE *file = NULL;
  file = fopen(ruta, "wb");
  if (file)
  {
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fclose(file);
    cout << "Creacion exitosa de particcion exitosa" << endl;
  }
  else
  {
    perror("ERROR: No existe el archivo en la ruta #");
    cout << ruta << "#" << endl;
  }
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

Particion crearParticion(int size, string name, char tipo, char fit, int inicio, char unidad)
{
  if (unidad == 'k')
  {
    size = size * 1024;
  }
  else
  {
    size = size * 1024 * 1024;
  }
  Particion newParticion;
  newParticion.part_fit = fit;
  strcpy(newParticion.part_name, name.c_str());
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
  }
  return -1;
}

int espacioTotalDisponible(MBR mbr)
{
  int espacioDisponible = mbr.mbr_tamano;
  int espacioOcupado = sizeof(mbr);
  for (int i = 0; i < 4; i++)
  {
    espacioOcupado += mbr.mbr_partition[i].part_size;
  }
  espacioDisponible = espacioDisponible - espacioOcupado;
  return espacioDisponible;
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

MBR obtenerMbr(string path)
{
  MBR mbr_;
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());

  FILE *file = NULL;
  file = fopen(ruta, "rb");
  if (file)
  {
    fseek(file, 0, SEEK_SET);
    fread(&mbr_, sizeof(MBR), 1, file);
    fclose(file);
    return mbr_;
  }
  cout << "ERROR: No existe el archivo en la ruta #" << ruta << "#" << endl;
  return mbr_;
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
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());
  mkdir(path);
  FILE *file = NULL;
  file = fopen(ruta, "wb");
  if (!file)
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
  }
  if (sizeof(mbr) < size)
  {
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fflush(file);
    fclose(file);
    cout << "Creación de disco exitosa!!" << endl;
    cout << "Fecha de creacion de disco: " << asctime(gmtime(&mbr.mbr_fecha_creacion)) << endl;
  }
}

void fdisk(float size, char unidad, string path, char type, char fit, string name)
{
  MBR mbr = obtenerMbr(path);
  if (mbr.mbr_tamano > 0)
  {
    int espacio = espacioTotalDisponible(mbr);
    cout << "Espacio disponible del disco:" << espacio << endl;
    int e_disponiblePos = espacioDisponible(mbr);
    int s = 0;
    if (unidad = 'k')
    {
      s = size * 1024;
    }
    else
    {
      s = size * 1024 * 1024;
    }
    espacio -= s;
    if (e_disponiblePos != -1 && espacio > 0)
    {
      if (nombreDisponible(mbr.mbr_partition, name))
      {
        int inicio = getInicioParticion(mbr, size);
        if (inicio != -1)
        {
          Particion nuevaParticion = crearParticion(size, name, type, fit, inicio, unidad);
          mbr.mbr_partition[e_disponiblePos] = nuevaParticion;
          escribirMBR(mbr, path);
        }
        else
        {
          cout << "ERROR: No hay espacio para la partición " << name << endl;
        }
      }
      else
      {
        cout << "ERROR: Ya existe una particion " << name << " en este disco " << endl;
      }
    }
    else
    {
      cout << "Error: No se pueden hacer más particiones por falta de espacio o cantidad" << endl;
    }
  }
  else
  {
    cout << "ERROR: Error al obtener el mbr" << endl;
  }
}

//fdisk -Size=1 -path=discos/disco3.disk -name=Particion1
