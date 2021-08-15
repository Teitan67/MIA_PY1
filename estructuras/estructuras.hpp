#include <string.h>
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
    string path;
    int size;
    char ajuste;
    char unidad;
};
