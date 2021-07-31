#include <iostream>
#include "string"
using namespace std;

int main()
{
    bool ejecucion=true; 
    string entrada;
    string listaComandos=
    "LISTA DE COMANDOS VALIDOS\n"
    "1. Salir: Termina la ejecuciòn del programa.\n"
    "2. \"?\" Despliega lista de comandos validos.\n";
    cout << "Iniciando sistema simulado ext4 v0.1 autor Teitan67..."<< endl;
    cout << "Oprime \"?\" y Enter para màs informaciòn"<< endl;
    do
    {
        cout << "Ingrese un comando: ";
        cin >> entrada; 
        //Automata para separar las diferentes entradas
        if(entrada=="salir"){
            cout<<"Finalizando ejecucion..."<<endl;
            ejecucion=false;
        }else if(entrada=="?"){
            cout << listaComandos<<endl;
        }else{
            cout<<"ERROR: Comando no reconocido!"<< endl<<endl;
        }
    }
    while(ejecucion);
    
    return EXIT_SUCCESS;
}