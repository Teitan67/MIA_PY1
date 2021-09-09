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
#include <iomanip>
#include <ctime>
#include <chrono>

#include "../estructuras/estructuras.hpp"

using namespace std;

vector<ParicionMontada> montadas;

Codigo generarCodigo(vector<ParicionMontada> particionesMontadas, string path)
{
  Codigo cod;
  for (auto &&particionMontada : particionesMontadas)
  {
    cod.numero = cod.numero + 1;
    if (strcasecmp(particionMontada.path.c_str(), path.c_str()))
    {
      break;
    }
  }
  for (auto &&particionMontada : particionesMontadas)
  {
    if (strcasecmp(particionMontada.path.c_str(), path.c_str()))
    {
      cod.letra = cod.letra + 1;
    }
  }
  cod.cod = to_string(cod.carnet) + to_string(cod.numero) + char(cod.letra);

  return cod;
}
//mount -path=discos/disco3.disk -name=Particion2
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

bool particionExtendidaDisponible(Particion partciones[4], char tipo)
{
  for (size_t i = 0; i < 4; i++)
  {
    if (partciones[i].part_type == tipo && partciones[i].part_type == 'e')
    {
      return false;
    }
  }
  return true;
}

Particion getParticionExtendida(Particion partciones[4])
{
  for (size_t i = 0; i < 4; i++)
  {
    if (partciones[i].part_type == 'e')
    {
      return partciones[i];
    }
  }
  Particion pVacia;
  return pVacia;
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

int getDisponibleExtendida(Particion extendida, string path)
{
  int disponible = extendida.part_size;
  MBR mbr_ = obtenerMbr(path);
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");
  strcpy(ruta, path.c_str());

  FILE *file = NULL;
  file = fopen(ruta, "rb");
  if (file)
  {
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr_, sizeof(MBR), 1, file);
    fclose(file);
    for (int i = 0; i < mbr_.noLogicas; i++)
    {
      cout << "Disponible " << disponible << " logica de size: " << mbr_.logicas[i].part_size << endl;
      disponible -= mbr_.logicas[i].part_size;
    }
    cout << "Disponible " << disponible << endl;
  }
  return disponible;
}
string convertToString(char *a)
{
  int i;
  string s = "";
  for (i = 0; i < strlen(a); i++)
  {
    s = s + a[i];
  }
  return s;
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
  int espacioOcupado = sizeof(mbr) - sizeof(Particion) * 100;
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

void imprimirLogicas(string path)
{
  char ruta[path.size() + 1];
  strcpy(ruta, path.c_str());

  FILE *file = NULL;
  file = fopen(ruta, "rb+");
  if (file)
  {
    MBR mbr;
    fseek(file, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, file);
    cout << "Numero de particiones: " << mbr.noLogicas << endl;

    for (int i = 0; i < 4; i++)
    {
      cout << "Inicio Particion: " << mbr.mbr_partition[i].part_start << " Size actual:" << mbr.mbr_partition[i].part_size << endl;
    }
    cout << endl
         << "PARICIONES LOGICAS" << endl;
    for (int i = 0; i < mbr.noLogicas; i++)
    {
      cout << "Nombre de particion: " << mbr.logicas[i].part_name << endl;
    }
  }
}

void crearParticionLogica(MBR mbr, string path, int size, string name, char type, char fit, char unidad)
{
  Particion pExtendida = getParticionExtendida(mbr.mbr_partition);

  char ruta[path.size() + 1];
  strcpy(ruta, path.c_str());

  FILE *file = NULL;
  file = fopen(ruta, "wb");
  if (file)
  {
    Particion logica;
    logica.part_status = 'c';
    logica.part_type = 'l';
    logica.part_fit = fit;
    if (mbr.noLogicas > 0)
    {
      logica.part_start = mbr.logicas[mbr.noLogicas - 1].part_start + mbr.logicas[mbr.noLogicas - 1].part_size;
    }
    else
    {
      logica.part_start = sizeof(Particion) + pExtendida.part_start;
    }

    logica.part_size = size;
    strcpy(logica.part_name, name.c_str());

    mbr.logicas[mbr.noLogicas] = logica;
    int nuevoValor = mbr.noLogicas + 1;
    mbr.noLogicas = nuevoValor;
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fflush(file);
    fclose(file);
    escribirMBR(mbr, path);
  }
  else
  {
    perror("Creacion de logica error: ");
  }
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
  mbr.noLogicas = 0;
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
    else if (unidad = 'b')
    {
      s = size;
    }
    else
    {
      s = size * 1024 * 1024;
    }
    if (type == 'l')
    {
      Particion particionExtendida = getParticionExtendida(mbr.mbr_partition);
      cout << "Validacion extendida " << particionExtendida.part_size << endl;
      if (particionExtendida.part_size > 0)
      {
        int disponibleExtendida = getDisponibleExtendida(particionExtendida, path);
        if (disponibleExtendida > size)
        {
          crearParticionLogica(mbr, path, s, name, type, fit, unidad);
          cout << "Creacion de particion logica correcta" << endl;
        }
      }
      else
      {
        cout << "ERROR: No hay particion extendida para crear una lógica" << endl;
      }
    }
    else
    {
      espacio -= s;
      if (e_disponiblePos != -1 && espacio > 0)
      {
        if (nombreDisponible(mbr.mbr_partition, name))
        {
          if (particionExtendidaDisponible(mbr.mbr_partition, type))
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
            cout << "ERROR: Ya existe una particion extendida dentro del disco" << endl;
          }
        }
        else
        {
          cout << "ERROR: Ya existe una particion " << name << " en este disco " << endl;
        }
      }
      else
      {
        cout << "ERROR: No se pueden hacer más particiones por falta de espacio o cantidad" << endl;
      }
    }
  }
  else
  {
    cout << "ERROR: Error al obtener el mbr" << endl;
  }
}
bool validarParticion(MBR mbr, string name)
{
  char r[name.size()];
  strcpy(r, name.c_str());
  for (auto &&particion : mbr.mbr_partition)
  {
    if (strcasecmp(r, particion.part_name))
    {
      return true;
    }
  }

  return false;
}
float porcentaje(int cantidad, int total)
{

  return cantidad * 100.000 / total;
}
void crearReporte(string contenido, string path)
{
  char ruta[path.size() + 1];
  path.replace(path.size() - 1, 1, "");

  char txt[contenido.size() + 1];
  strcpy(txt, contenido.c_str());
  FILE *file = fopen("disco.txt", "w");
  fputs(txt, file);
  fclose(file);
  system("dot -Tpng disco.txt -o IMG.png");
}
void reporteDeDisco(string pathReporte, string id)
{
  cout << "Iniciando creacion" << endl;
  string path = "";
  for (auto &&pMontadas : montadas)
  {
    if (id.compare(pMontadas.cod.cod))
    {
      path = pMontadas.path;
    }
  }

  MBR mbr = obtenerMbr(path);
  int total = sizeof(mbr) - sizeof(Particion) * 100;
  float mbrSize = porcentaje(total, mbr.mbr_tamano);
  total = mbr.mbr_tamano;
  string codGrapViz = "";
  Particion *particiones = burbuja(mbr.mbr_partition);
  int inicio = sizeof(MBR);

  codGrapViz += "digraph structs {\ndisco [shape=record,label=\"";
  codGrapViz += "MBR\\n " + to_string(mbrSize) + "% |";
  string extendidas = "";
  int p1 = particiones[0].part_start - inicio;
  for (int i = 0; i < 4; i++)
  {
    if (p1 > 0)
    {
      codGrapViz += "Libre \\n " + to_string(porcentaje(p1, total)) + "% \\n |";
    }
    if (particiones[i].part_size > 0)
    {
      if (particiones[i].part_type == 'e')
      {
        codGrapViz += "{" + convertToString(particiones[i].part_name) + "|";
        extendidas = "EBR";
        for (auto &&logica : mbr.logicas)
        {
          if (logica.part_size > 0)
          {
            extendidas += "| " + convertToString(logica.part_name) + " \n " + to_string(porcentaje(logica.part_size, total)) + "%|EBR";
          }
        }
        codGrapViz += extendidas;
        codGrapViz += "} |";
      }
      else
      {
        codGrapViz += convertToString(particiones[i].part_name) + " \\n " + to_string(porcentaje(particiones[i].part_size, total)) + "% |";
      }
    }
    p1 = particiones[i + 1].part_start - particiones[i].part_start - particiones[i].part_size;
  }

  p1 = total - particiones[3].part_start - particiones[3].part_size + sizeof(Particion) * 100;
  if (p1 > 0)
  {
    codGrapViz += "Libre \\n " + to_string(porcentaje(p1, total)) + "% |";
  }

  codGrapViz += "";
  codGrapViz += "\\n\"]}";
  crearReporte(codGrapViz, pathReporte);
  cout << codGrapViz << endl;
}

void mount(string path, string name)
{
  Codigo cod_ = generarCodigo(montadas, path);
  MBR mbrAux = obtenerMbr(path);
  ParicionMontada nuevaMontada = {cod_, path, name, mbrAux};
  if (validarParticion(mbrAux, name))
  {
    montadas.push_back(nuevaMontada);
    cout << "Path"
         << "|"
         << "Nombre Particion"
         << "|"
         << "COD" << endl;
    for (auto &&i : montadas)
    {
      cout << i.path << "|" << i.name << "|" << i.cod.cod << endl;
    }
  }
}

void unmount(string id)
{
  int index = -1;
  for (auto &&i : montadas)
  {

    cout << "ID:" << id << " Comparando: " << i.cod.cod << endl;
    if (i.cod.cod.compare(id) == 1)
    {
      cout << "ACTIVADO" << endl;
      montadas.erase(montadas.begin() + index);
    }

    ++index;
  }
  cout << "Path"
       << "|"
       << "Nombre Particion"
       << "|"
       << "COD" << endl;
  for (auto &&i : montadas)
  {
    cout << i.path << "|" << i.name << "|" << i.cod.cod << endl;
  }
}
void reporteMbr(string pathReporte, string id)
{
  cout << "Iniciando creacion" << endl;
  string path = "";
  for (auto &&pMontadas : montadas)
  {
    if (id.compare(pMontadas.cod.cod))
    {
      path = pMontadas.path;
    }
  }

  MBR mbr = obtenerMbr(path);

  string codGrapViz = "digraph structs {";
  codGrapViz += "mbr[shape=record,label=\"{nombre|mbr_tamaño|mbr_fechaCreacion|mbr_disk_signature|disk_fit";
  int i = 0;
  for (auto &&particion_ : mbr.mbr_partition)
  {
    if (particion_.part_size > 0)
    {
      codGrapViz += "|part_status_" + to_string(i) + "|part_type_" + to_string(i) + "|part_fit_" + to_string(i) + "|part_stax_" + to_string(i) + "|part_size_" + to_string(i) + "|part_name_" + to_string(i) + "";
    }
    i++;
  }
  codGrapViz += "}|";

  time_t seconds = mbr.mbr_fecha_creacion;

  std::stringstream ss;
  ss << seconds;
  std::string ts = ss.str();

  codGrapViz += "{" + convertToString(mbr.mbr_id) + "_disk|" + to_string(mbr.mbr_tamano) + "|" + "|" + to_string(mbr.mbr_disk_signature) + "|" + to_string(mbr.disk_fit);

  for (auto &&particion_ : mbr.mbr_partition)
  {
    if (particion_.part_size > 0)
    {
      codGrapViz += "|" + to_string(particion_.part_status) + "|" + particion_.part_type + "|" + particion_.part_fit + "|" + particion_.part_fit + "|" + to_string(particion_.part_size) + "|" + particion_.part_name + "";
    }
  }
  codGrapViz += "}\"];";

  i = 0;
  for (auto &&logica : mbr.logicas)
  {
    if (logica.part_size > 0)
    {
      codGrapViz += "" + convertToString(logica.part_name) + " [shape=record,label=\" {part_name_" + to_string(i) + "|part_start_" + to_string(i) + "|part_size_" + to_string(i) + "|part_next" + to_string(i) + "|part_status_" + to_string(i) + "|part_type_" + to_string(i) + "|part_fit_" + to_string(i) + " }|{" + convertToString(logica.part_name) + "|" + to_string(logica.part_start) + "|" + to_string(logica.part_size) + "|" + to_string(logica.part_start + logica.part_size) + "|" + logica.part_status + "|" + logica.part_type + "|" + logica.part_fit + "}\"];";
    }
    i++;
  }

  codGrapViz += "}";
  crearReporte(codGrapViz, pathReporte);
  cout << codGrapViz << endl;
}

void borrarParticion(string name, string path)
{
  char r[name.size()];
  strcpy(r, name.c_str());
  MBR mbrAux = obtenerMbr(path);
  for (auto &&i : mbrAux.mbr_partition)
  {
    cout<<"comarar "<<r<<"-"<<i.part_name<<endl;
    if (!strcasecmp(r, i.part_name))
    {
      cout<<"ACTIVADO"<<endl;
      i.part_size = 0;
      if (i.part_type == 'e')
      {
        for (auto &&a : mbrAux.logicas)
        {
          a.part_size = 0;
        }
      }
    }
  }
  for (auto &&i : mbrAux.logicas)
  {
    if (!strcasecmp(r, i.part_name))
    {
      i.part_size = 0;
    }
  }
  
  escribirMBR(mbrAux, path);
}
//fdisk -Size=1 -path=discos/disco3.disk -name=Particion1
//mount -path=discos/disco3.disk -name=Particion2
//rep -id=440A -name=mbr -path=discos/rep.png
//fdisk -path=discos/disco3.disk -name=Particion1 -delete=full
//fdisk -Size=1 -path=disco.disk -name=Particion1
//mount -path=disco.disk -name=Particion1
