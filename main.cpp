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

    // Inicializando o programa em IA32
    translator.initializateCode();

    // Traduzindo o Assembly
    // Primeira passagem para definir os espacos de memoria
    for (int i = 0; i < program_source.size(); ++i)
    {
        // Adquirindo o tamanho da intrucao
        int memSpcae = getMemorySpace(program_source[i]);

        if( !translator.hasStopped() )
        {
            switch (stoi(program_source[i]))
            {
                case 14: // STOP
                    translator.setStopped(true);
                    break;
                default:
                    break;
            }
        } else {
            translator.addMemory(i, program_source[i]);
        }
    }

    // Checa as variaveis e adiciona elas no codigo final
    translator.createVariables();

    // Segunda passagem para gerar o codigo final
    translator.setStopped(false);
    for (int i = 0; i < program_source.size(); ++i)
    {

        // Adquirindo o tamanho da intrucao
        int memSpcae = getMemorySpace(program_source[i]);

        if( !translator.hasStopped() )
        {
            switch (stoi(program_source[i]))
            {
                case 1:  // ADD
                    i++;
                    translator.ADD(program_source[i]);
                    break;
                case 2:  // SUB
                    i++;
                    translator.SUB(program_source[i]);
                    break;
                case 3:  // MUL
                    i++;
                    translator.MUL(program_source[i]);
                    break;
                case 4:  // DIV
                    i++;
                    translator.DIV(program_source[i]);
                    break;
                case 5:  // JMP
                    i++;
                    break;
                case 6:  // JMPN
                    i++;
                    break;
                case 7:  // JMPP
                    i++;
                    break;
                case 8:  // JMPZ
                    i++;
                    break;
                case 9:  // COPY
                    translator.COPY(program_source[i + 1],program_source[i + 2]);
                    i += 2;
                    break;
                case 10: // LOAD
                    i++;
                    break;
                case 11: // STORE
                    i++;
                    break;
                case 12: // INPUT
                    i++;
                    translator.INPUT(program_source[i]);
                    break;
                case 13: // OUTPUT
                    i++;
                    break;
                case 14: // STOP
                    translator.STOP();
                    break;
                default: // Unknown opcode
                    translator.addWarning("OPCODE " + program_source[i] + " nao identificado lol");
                    break;
            }
        }
    }


    // Parte Opcional
    // Adicionar novos OPCODES ( S_INPUT | S_OUTPUT )


    // Mostrando os resultados
    system("clear");
    cout << "| Lucas Pena [13/0056162]\n"
    << "| Tradutor de Assembly Inventado para IA-32.\n"
    << "----------------------------------------------\n\n"
    << "Programa inserido\n---------------------------\n ";

    for (auto i = program_source.begin(); i != program_source.end(); ++i)
    {
        cout << *i << " ";
    }

    cout << "\n\nCodigo gerado \n---------------------------\n";
    cout << translator.getCode(); 
    cout << "\n" << endl << "\nWarnings\n---------------------------\n";
    if(translator.getWarnings() == "")
    {
        cout << " Sem warnings.";
    } else {
        cout << translator.getWarnings();
    }
    cout << "\n\nMemoria\n---------------------------\n";
    translator.showMemory();
    // << "Aperte enter para sair..." << endl;
    // cin.get();

    return 0;
}