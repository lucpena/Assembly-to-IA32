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
    // Primeira passagem para definir os espacos de memoria e as Labels
    for (int i = 0; i < program_source.size(); ++i)
    {
        // Adquirindo o tamanho da intrucao
        int memSpcae = getMemorySpace(program_source[i]);

        // Se o programa encerrar, tudo depois eh espaco de memoria.
        // Se for um pulo, salva o destino como uma Label
        if( !translator.hasStopped() )
        {
            switch (stoi(program_source[i]))
            {
                case 5:  // JMP
                    i++;
                    translator.addLog("JMP para " + program_source[i]);
                    translator.addMemoryLabel(stoi(program_source[i]));
                    break;
                case 6:  // JMPN
                    i++;
                    translator.addLog("JMPN para " + program_source[i]);
                    translator.addMemoryLabel(stoi(program_source[i]));
                    break;
                case 7:  // JMPP
                    i++;
                    translator.addLog("JMPP para " + program_source[i]);
                    translator.addMemoryLabel(stoi(program_source[i]));
                    break;
                case 8:  // JMPZ
                    i++;
                    translator.addLog("JMPZ para " + program_source[i]);
                    translator.addMemoryLabel(stoi(program_source[i]));
                    break;
                case 12: // INPUT
                    translator.setHasInput(true);
                    translator.addLog("Input: " + program_source[i + 1]);
                    translator.addMemory(stoi(program_source[i + 1]), "0");
                    break;
                case 13: // OUTPUT
                    translator.setHasOutput(true);
                    translator.addLog("Output: " + program_source[i + 1]);
                    translator.addMemory(stoi(program_source[i + 1]), "0");
                    break;
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

        // Adquirindo o tamanho da instrucao
        int memSpcae = getMemorySpace(program_source[i]);

        if( !translator.hasStopped() )
        {
            // Adiciona uma label caso encontre Jumps
            if( translator.getLabels().size() != 0 )
            {
                for( int label : translator.getLabels() )
                {
                    if( label == i )
                    {
                        translator.addLabel("LABEL" + to_string(i));
                    }
                }
            }

            // Traducao
            string status = "";

            switch (stoi(program_source[i]))
            {
                case 1:  // ADD
                    translator.addText("; ADD");
                    i++;
                    translator.ADD(program_source[i]);
                    break;
                case 2:  // SUB
                    translator.addText("; SUB");
                    i++;
                    translator.SUB(program_source[i]);
                    break;
                case 3:  // MUL
                    translator.addText("; MUL");
                    i++;

                    status = translator.MUL(program_source[i]);

                    if( status == "Overflow")
                    {
                        translator.addWarning("OVERFLOW DETECTADO!!");
                    }

                    status = "";
                    break;
                case 4:  // DIV
                    translator.addText("; DIV");
                    i++;
                    translator.DIV(program_source[i]);
                    break;
                case 5:  // JMP
                    translator.addText("; JMP");
                    i++;
                    translator.JMP(program_source[i]);
                    break;
                case 6:  // JMPN
                    translator.addText("; JMPN");
                    i++;
                    translator.JMPN(program_source[i]);
                    break;
                case 7:  // JMPP
                    translator.addText("; JMPP");
                    i++;
                    translator.JMPP(program_source[i]);
                    break;
                case 8:  // JMPZ
                    translator.addText("; JUMPZ");
                    i++;
                    translator.JMPZ(program_source[i]);
                    break;
                case 9:  // COPY
                    translator.addText("; COPY");
                    translator.COPY(program_source[i + 1],program_source[i + 2]);
                    i += 2;
                    break;
                case 10: // LOAD
                    translator.addText("; LOAD");
                    i++;
                    translator.LOAD(program_source[i]);
                    break;
                case 11: // STORE
                    translator.addText("; STORE");
                    i++;
                    translator.STORE(program_source[i]);
                    break;
                case 12: // INPUT
                    translator.addText("; INPUT");
                    i++;
                    translator.INPUT(program_source[i]);
                    break;
                case 13: // OUTPUT
                    translator.addText("; OUTPUT");
                    i++;
                    translator.OUTPUT(program_source[i]);
                    break;
                case 14: // STOP
                    translator.addText("; STOP");
                    translator.STOP();
                    break;
                default: // Unknown opcode
                    translator.addWarning("OPCODE " + program_source[i] + " nao identificado lol");
                    break;
            }
        }
    }

    // Checar se houve INPUT ou OUTPUT para adicionar estas funcoes no final do programa
    translator.checkFunctions();

    // Salvando o programa em um arquivo texto
    
    string inputName = argv[1];
    string outputName = "out-" + inputName.substr(0, inputName.find_last_of('.')) + ".asm";
    ofstream out(outputName);
    out << translator.getCode();
    out.close();

    // Mostrando os resultados
    system("clear");
    cout 
    << "| Lucas Pena [13/0056162]\n"
    << "| Tradutor de Assembly Inventado para IA-32.\n"
    << "----------------------------------------------";
    // << "Programa inserido\n---------------------------\n ";

    // for (auto i = program_source.begin(); i != program_source.end(); ++i)
    // {
    //     cout << *i << " ";
    // }

    cout << endl << "\nWarnings\n---------------------------\n";
    if(translator.getWarnings() == "")
    {
        cout << " Sem warnings.";
    } else {
        cout << translator.getWarnings();
    }

    if(translator.getLog() != "")
    {
        cout << "\n" << endl << "\nLog\n---------------------------\n";
        cout << translator.getLog();
    }

    cout << "\n\nMemoria\n---------------------------\n";
    translator.showMemory();

    
    cout << "\n\nCodigo gerado \n---------------------------\n";
    cout << translator.getCode(); 

    cout << "\n\n" <<endl;
    // << "Aperte enter para sair..." << endl;
    // cin.get();

    return 0;
}