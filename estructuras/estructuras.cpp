#include <string.h>
#include "string"salir
#include <time.h>
using namespace std;
struct Particion
{
    char part_status;
    char part_type = 'p'; // primaria, extendida, logica
    char part_fit = 'f'; //las opciones por default
    int part_start;
    int part_size;
    char part_name[16];
};
struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Particion mbr_partition[4];
};
struct Disco
{
    string path = "nulo";   // Ruta predestinada del disco
    int size = -1;          // Tamaño predeterminado
    char ajuste = 'f';      // F primer ajuste W peor ajuste B mejor ajuste predeterminado F
    char unidad = 'm';      // Unidad puede ser m o k predeterminado  m
};
