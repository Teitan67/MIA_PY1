
using namespace std;
struct Particion
{
    char part_status= 'd';  //d desconectada
    char part_type =  'p'; // primaria, extendida, logica
    char part_fit = 'f'; //las opciones por default
    int part_start=-1;
    int part_size=-1;
    char part_name [30];
};

struct MBR
{
    char mbr_id[30];
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Particion mbr_partition[4];
    int noLogicas=0;
};

struct Disco
{
    char * path;
    int size;
    char ajuste;
    char unidad;
};
