#include <string>
#include "../estructuras/estructuras.hpp"
using namespace std;
MBR getMBR(string );
int getInicioParticion(MBR mbr, float size_partition);
Particion crearParticion(int size, string name, char tipo, char fit, int inicio);
int espacioDisponible(MBR mbr);
void mkdir(string );
void mkdisk( string ,int , char ,char  );
//size, u,string,type,f,name
void fdisk(float,char,string,char,char,string);
