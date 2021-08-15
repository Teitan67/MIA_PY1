#include <iostream>
#include "analizador.hpp"
#include <vector>
#include <sstream>
#include "string"
#include <cstdlib>
#include <string>
#include "../comandos/comandos.hpp"

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
    else if (comando[0] == "mkdisk ")
    {
        cout << "Creando disco..." << endl;
        //mkdisk -size=12 -path=hola/das
        string path = "nulo"; // Ruta predestinada del disco
        int size = -1;     // Tamaño predeterminado
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
                mkdisk( path, size,  ajuste, unidad);
            }
        }
        else
        {
            cout << "ERROR: cantidad de parametros insuficientes " << endl;
        }
    }
    else
    {
        cout << "ERROR: Comando no reconocido! #" << comando[0] << "#" << endl
             << endl;
    }
    return true;
}
