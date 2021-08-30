#include <iostream>
#include "analizador/analizador.hpp"
#include "string"
using namespace std;

int main()
{
    bool ejecucion = true;
    string entrada;
    cout << "Iniciando sistema simulado ext4 v0.1 autor Teitan67..." << endl;
    cout << "Oprime \"?\" y Enter para más informaciòn" << endl;
    do
    {
        cout << "Ingrese un comando: ";
        getline(cin, entrada, '\n');
        cout<<endl;
        //Automata para separar las diferentes entradas
        ejecucion = analizar(entrada);
    } while (ejecucion);

    return EXIT_SUCCESS;
}