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

    Translator translator;
    
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
    for (int i = 0; i != program_source.size(); ++i)
    {
       cout << program_source[i] << " ";

       // Adquirindo o tamanho da intrucao
       int memSpcae = getMemorySpace(program_source[i]);

       switch (stoi(program_source[i]))
       {
            case 1:  // ADD
                break;
            case 2:  // SUB
                break;
            case 3:  // MUL
                break;
            case 4:  // DIV
                break;
            case 5:  // JMP
                break;
            case 6:  // JMPN
                break;
            case 7:  // JMPP
                break;
            case 8:  // JMPZ
                break;
            case 9:  // COPY
                break;
            case 10: // LOAD
                break;
            case 11: // STORE
                break;
            case 12: // INPUT
                break;
            case 13: // OUTPUT
                break;
            case 14: // STOP
                break;
            default: // Unknown opcode
                break;
       }
    }

    // Parte Opcional
    // Adicionar novos OPCODES ( S_INPUT | S_OUTPUT )


    // Mostrando os resultados
    system("clear");
    cout << endl;
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