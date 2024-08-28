/*
    Lucas Araujo Pena - 13/0056162

    Segundo trabalho de Software Basico

    Tradutor de Assembly para Intel IA-32

*/

#include "system.hpp"

int main( int argc, char *argv[] )
{
    // Receber o nome do arquivo
    if( argc < 2 )
    {
        cout << "\nUsando o programa: " << argv[0] << " programa.obj \n" << endl;
        return -1;
    }

    // Ler arquivo com o codigo fonte
    string fileName = argv[1];
    string fileLine = "";

    Tradutor tradutor;
    
    ifstream input(fileName);

    if(input.is_open())
    {

        string line, token;
        vector<string> lineContent = {};

        // Lendo todo o arquivo texto
        while(getline(input, line))
        {
            stringstream ss(line);

            // Le um token por vez, tirando o espaco
            while( ss >> token)
            {
                program_source.push_back(token);
            }
        }
    }
    input.close();


    // Traduzindo o Assembly
    for (auto i = program_source.begin(); i != program_source.end(); ++i)
    {
        
    }

    // Parte Opcional
    // Adicionar novos OPCODES ( S_INPUT | S_OUTPUT )


    // Mostrando os resultados
    system("clear");
    cout << endl;
    cout << " Lucas Pena [13/0056162]\n";
    cout << " Tradutor de Assembly Inventado para IA-32.\n";
    cout << "----------------------------------------------\n\n";
    cout << "Programa inserido: ";
    for (auto i = program_source.begin(); i != program_source.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
    cin.get();

    return 0;
}