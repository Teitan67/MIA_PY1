#include <iostream>
#include "analizador.hpp"
#include <vector>
#include <sstream>
#include "string"
#include <cstdlib>
#include <string>
#include "../comandos/comandos.hpp"
#include <fstream>

using namespace std;
string listaComandos =
    "LISTA DE COMANDOS VALIDOS\n"
    "1. Salir: Termina la ejecuciòn del programa.\n"
    "2. \"?\" Despliega lista de comandos validos.\n"
    "Espero haber sido de ayuda :D\n";

vector<string> split(string str, char pattern)
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

bool analizar(string entrada)
{

    //Separamos el comando en un arreglo de cadenas
    vector<string> comando = split(entrada, '-');

    std::for_each(comando[0].begin(), comando[0].end(), [](char &c)
                  { c = ::tolower(c); });

    if (comando[0] == "salir")
    {
        cout << "Finalizando ejecucion..." << endl;
        return false;
    }
    else if (comando[0] == "?")
    {
        cout << listaComandos << endl;
    }
    else if (comando[0] == "rep ")
    {
        string id = "";
        string path = "";
        string name = "";
        for (size_t i = 1; i < comando.size(); i++)
        {
            vector<string> parametro = split(comando[i], '=');
            if (parametro.size() == 2)
            {
                //Parametro

                std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                              { c = ::tolower(c); });
                if (parametro[0] == "id")
                {
                    id = parametro[1];
                }
                else if (parametro[0] == "path")
                {
                    path = parametro[1];
                }
                else if (parametro[0] == "name")
                {
                    name = parametro[1];
                }
            }
            else
            {
                cout << "ERROR: No hay congruencia en este parametro: " << comando[i] << endl;
            }
        }
        if (name.compare("mbr")==1)
        {
            reporteMbr(path,id);
        }
        else if(name.compare("disk")==1)
        {
            reporteDeDisco(path,id);
        }
        else
        {
            cout<<"Error name no valido #"<<name<<"#"<<endl;
        }
        //mount -path=discos/disco3.disk -name=Particion2
        //rep -id=561A -Path=/home/user/reports/reporte1.jpg -name=mbr 
        //rep -id=440A -name=disk -Path=/home/user/reports/reporte1.jpg 
    }
    else if (comando[0] == "mkdisk ")
    {
        //cout << "Creando disco..." << endl;
        //mkdisk -size=12 -path=hola/das
        string path = "nulo"; // Ruta predestinada del disco
        int size = -1;        // Tamaño predeterminado
        char ajuste = 'f';    // F primer ajuste W peor ajuste B mejor ajuste predeterminado F
        char unidad = 'm';    // Unidad puede ser m o k predeterminado  m
        if (comando.size() >= 3)
        {
            for (size_t i = 1; i < comando.size(); i++)
            {
                vector<string> parametro = split(comando[i], '=');
                if (parametro.size() == 2)
                {
                    //Parametro

                    std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                                  { c = ::tolower(c); });
                    if (parametro[0] == "path")
                    {
                        path = parametro[1];
                    }
                    else if (parametro[0] == "size")
                    {
                        size = stod(parametro[1]);
                    }
                    else if (parametro[0] == "u")
                    {
                        unidad = parametro[1].at(0);
                    }
                    else if (parametro[0] == "f")
                    {
                        ajuste = parametro[1].at(0);
                    }
                }
                else
                {
                    cout << "ERROR: No hay congruencia en este parametro: " << comando[i] << endl;
                }
            }

            if (size < 0)
            {
                cout << "ERROR: No se ingreso un size " << endl;
            }
            else if (path == "nulo")
            {
                cout << "ERROR: No se ingreso un path" << endl;
            }
            else
            {
                mkdisk(path, size, ajuste, unidad);
            }
        }
        else
        {
            cout << "ERROR: cantidad de parametros insuficientes " << endl;
        }
    }
    else if (comando[0] == "mount ")
    {
        string name = "";
        string path = "";
        for (size_t i = 1; i < comando.size(); i++)
        {
            vector<string> parametro = split(comando[i], '=');
            if (parametro.size() == 2)
            {
                //Parametro
                std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                              { c = ::tolower(c); });
                if (parametro[0] == "path")
                {
                    path = parametro[1];
                }
                else if (parametro[0] == "name")
                {
                    name = parametro[1];
                }
            }
            else
            {
                cout << "ERROR: No hay congruencia en este parametro: " << comando[i] << endl;
            }
        }
        mount(path, name);
        //fdisk -Size=300 -path=/home/Disco1.disk -name=Particion1
        //exec -path=entrada.txt
    }
    else if (comando[0] == "unmount ")
    {
        string id = "";
        for (size_t i = 1; i < comando.size(); i++)
        {
            vector<string> parametro = split(comando[i], '=');
            if (parametro.size() == 2)
            {
                //Parametro
                std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                              { c = ::tolower(c); });
                if (parametro[0] == "id")
                {
                    id = parametro[1];
                }
            }
            else
            {
                cout << "ERROR: No hay congruencia en este parametro: " << comando[i] << endl;
            }
        }
        unmount(id);
    }
    else if (comando[0] == "rmdisk ")
    {
        if (comando.size() == 2)
        {
            vector<string> parametro = split(comando[1], '=');
            if (parametro.size() == 2)
            {
                std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                              { c = ::tolower(c); });
                string path;
                if (parametro[0] == "path")
                {
                    path = parametro[1];
                }
                string respuesta;
                cout << "¿Seguro que desea eliminar este disco " << path << "? (s/n) ";

                getline(cin, respuesta, '\n');
                if (respuesta == "s" || respuesta == "S")
                {
                    char ruta[path.size()];
                    strcpy(ruta, path.c_str());
                    //cout << "Eliminando *" << ruta << "*" << endl;
                    if (remove(ruta) != 0)
                    {
                        perror("Error al eliminar archivo");
                    }
                    else
                    {
                        puts("Archivo eliminado");
                    }
                }
            }
            else
            {
                cout << "ERROR: Datos erroneos" << endl;
            }
        }
        else
        {
            cout << "ERROR:Cantidad de parametros incorrecta" << endl;
        }
    }
    else if (comando[0] == "pausa " || comando[0] == "pausa")
    {
        string r = "";
        cout << "Escriba enter para continuar...";
        getline(cin, r, '\n');
    }
    else if (comando[0] == "exec ")
    {
        vector<string> parametro = split(comando[1], '=');
        if (parametro[0] == "path")
        {
            //cout << "Ejecutando... " << parametro[1] << endl;
            vector<string> comandos;
            ifstream input_file(parametro[1]);
            if (!input_file.is_open())
            {
                cerr << "No se puede abrir archivo - '"
                     << parametro[1] << "'" << endl;
                return EXIT_FAILURE;
            }
            string line;
            while (getline(input_file, line))
            {
                comandos.push_back(line);
            }

            for (const auto &i : comandos)
            {
                cout << i << endl;
                analizar(i);
            }
            input_file.close();
        }
        else
        {
            cout << "ERROR: Necesitamos un path y tenemos:" << parametro[0] << endl;
        }
    }
    else if (comando[0] == "fdisk ")
    {
        float size = 0;
        char u = 'k';
        string path = "nulo";
        char type = 'p';
        char f = 'w';
        string delete_ = "nulo";
        string name = "nulo";
        float add = 0;
        for (size_t i = 1; i < comando.size(); i++)
        {
            vector<string> parametro = split(comando[i], '=');
            if (parametro.size() == 2)
            {
                //Parametro

                std::for_each(parametro[0].begin(), parametro[0].end(), [](char &c)
                              { c = ::tolower(c); });
                if (parametro[0] == "path")
                {
                    path = parametro[1];
                }
                else if (parametro[0] == "name")
                {
                    name = parametro[1];
                }
                else if (parametro[0] == "size")
                {
                    size = stod(parametro[1]);
                }
                else if (parametro[0] == "u")
                {
                    u = parametro[1].at(0);
                }
                else if (parametro[0] == "type")
                {
                    type = parametro[1].at(0);
                }
                else if (parametro[0] == "f")
                {
                    f = parametro[1].at(0);
                }
                else if (parametro[0] == "delete")
                {
                    delete_ = parametro[1].at(0);
                }
                else if (parametro[0] == "add")
                {
                    add = stod(parametro[1]);
                }
            }
            else
            {
                cout << "ERROR: No hay congruencia en este parametro: " << comando[i] << endl;
            }
        }
        //fdisk -Size=300 -path=/home/Disco1.disk -name=Particion1
        //exec -path=entrada.txt
        if (delete_ == "nulo" && add == 0)
        {
            //cout << "Creando particion..." << endl;
            fdisk(size, u, path, type, f, name);
        }else if(delete_.compare("fast")||delete_.compare("full")){
            borrarParticion(name,path);
        }
        else
        {
            cout << "No se esta ejecutando nada de fdisk " << endl;
        }
    }
    else
    {
        cout << "ERROR: Comando no reconocido! #" << comando[0] << "#" << endl
             << endl;
    }
    cout << endl;
    return true;
}
//fdisk -path=discos/disco3.disk -name=Particion1 -delete=full