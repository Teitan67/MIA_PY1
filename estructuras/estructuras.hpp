
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
    Particion logicas[100];
    int noLogicas;
};

struct Disco
{
    char * path;
    int size;
    char ajuste;
    char unidad;
};

struct Codigo{
    int carnet=44;
    int letra=65;
    int numero=0;
    string cod=to_string(carnet)+to_string(numero)+char(letra);
};

struct ParicionMontada
{
    Codigo cod;
    string path="";
    string name="";
    MBR mbr;
};
