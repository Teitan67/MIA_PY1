#include <iostream>
#include "analizador.hpp"

using namespace std;
string listaComandos =
    "LISTA DE COMANDOS VALIDOS\n"
    "1. Salir: Termina la ejecuciòn del programa.\n"
    "2. \"?\" Despliega lista de comandos validos.\n"
    "Espero haber sido de ayuda :D\n";
bool analizar(string entrada)
{
    if (entrada == "salir")
    {
        cout << "Finalizando ejecucion..." << endl;
        return false;
    }
    else if (entrada == "?")
    {
        cout << listaComandos << endl;
    }
    else
    {
        cout << "ERROR: Comando no reconocido!" << endl
             << endl;
    }
    return true;
}
