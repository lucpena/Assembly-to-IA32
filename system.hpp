#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <limits>

using std::endl;
using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::stoi;
using std::to_string;
using std::find;

/********************************
    Configuracoes
*********************************/

map<string, int> opcodes = 
{
    {"ADD", 1}, {"SUB", 2}, {"MUL", 3}, 
    {"DIV", 4}, {"JMP", 5}, {"JMPN", 6}, 
    {"JMPP", 7}, {"JMPZ", 8}, {"COPY", 9}, 
    {"LOAD", 10}, {"STORE", 11}, {"INPUT", 12}, 
    {"OUTPUT", 13}, {"STOP", 14}
};

map<int, string> instructions = 
{
	{1, "ADD"}, {2, "SUB"}, {3, "MUL"},
	{4, "DIV"}, {5, "JMP"}, {6, "JMPN"},
	{7, "JMPP"}, {8, "JMPZ"}, {9, "COPY"},
	{10, "LOAD"}, {11, "STORE"}, {12, "INPUT"},
	{13, "OUTPUT"}, {14, "STOP"}
}; 

/********************************
    Tradutor
*********************************/
vector<string> program_source  = {};
map<int, string> inputs = {};

class Translator
{
private:

    int m_Accumulator;

    string m_IA32_data;
    string m_IA32_bss;
    string m_IA32_text;
    string m_IA32_text_functions;
    string m_Warnings;
    string m_Log;

    uint32_t m_AllocatedMemory;

    map<int, string> m_Memory;
    map<int, string> m_MemoryIA32;
    vector<int>      m_Labels;

    bool m_hasStopped;
    bool m_hasInput;
    bool m_hasOutput;

public:

    Translator():         
        m_IA32_data(""), 
        m_IA32_bss(""), 
        m_IA32_text(""),
        m_IA32_text_functions(""),
        m_Accumulator(0),
        m_Warnings(""),
        m_Log(""),
        m_AllocatedMemory(0),
        m_Memory({}),
        m_MemoryIA32({}),
        m_Labels({}),
        m_hasStopped(false),
        m_hasInput(false),
        m_hasOutput(false) {}

    ~Translator() {};

    void initializateCode();
    void createVariables();
    void addMemory( int add, string value );

    // inline string getAccumulator() { return m_Accumulator;}
    // inline void setAccumulator( string value ) { m_Accumulator = value; }

    void ADD( string value );
    void SUB( string value );
    string MUL( string value );
    void DIV( string value );
    void STORE( string value );
    void INPUT( string value );
    void OUTPUT( string value );
    void COPY( string dst, string src );
    void LOAD( string value );
    void JMP( string value );
    void JMPN( string value );
    void JMPP( string value );
    void JMPZ( string value );

    void STOP();

    void addData( string data, string space, string value );
    void addBSS( string bss );
    void addText( string text );
    void addFunction( string text );
    void addLabel( string label );
    void addMemoryLabel( int add );
    void addEqu( string data, string value );
    void checkFunctions();

    void addWarning( string value );
    void addLog( string value );
    void showMemory();

    string getMemoryValue( string add );
    string getLog();
    string getCode();
    string getWarnings();
    vector<int> getLabels(); 
    int getAcc();
    string getAccStr();

    void setAcc( string value );
    void setHasInput( bool value );
    void setHasOutput( bool value );
    
    bool hasStopped();
    void setStopped( bool value );

};
/********************************
    Definicoes do Tradutor
*********************************/

// Funcoes de traducao
inline void Translator::addLabel( string label )
{
    m_IA32_text += label + ":\n";
}

inline void Translator::addData( string data, string space, string value )
{
    m_IA32_data += "    " + data + " " + space + " " + value + "\n";
}

inline void Translator::addEqu( string data, string value )
{
    m_IA32_data += "    " + data + " equ " + value + "\n";
}

inline void Translator::addBSS( string bss )
{
    m_IA32_bss += "    " + bss + " resd 0\n";
}

inline void Translator::addText( string text )
{
    m_IA32_text += "    " + text + "\n";
}

inline void Translator::addFunction( string text)
{
    m_IA32_text_functions += text + "\n";
}

void Translator::checkFunctions()
{
    // Se tive input, adiciona ATOI (ASCII -> Integer)
    if(m_hasInput)
    {
        addFunction("atoi:");
        addFunction("   %define VALUE [EBP + 8]");
        addFunction("   enter 0,0");
        addFunction("   sub eax, eax");
        addFunction("   sub ebx, ebx");
        addFunction("   mov esi, VALUE");
        addFunction("   atoi_loop:");
        addFunction("       mov bl, byte[esi]");
        addFunction("       cmp bl, 0xA");
        addFunction("       je atoi_loop_end");
        addFunction("       cmp ebx, 0");
        addFunction("       je atoi_loop_end");
        addFunction("       sub bl, '0'");
        addFunction("       imul eax, eax, 10");
        addFunction("       add eax, ebx");
        addFunction("       inc esi");
        addFunction("       jmp atoi_loop");
        addFunction("   atoi_loop_end:");
        addFunction("   leave");
        addFunction("   ret 4");
        addFunction("");
    }

    // Se tiver output, adciona ITOA (Integer -> ASCII)
    if(m_hasOutput)
    {
        addData("buffer", "db", "0");

        addFunction("itoa:");
        addFunction("    enter 0,0");
        addFunction("    itoa_loop:");
        addFunction("        sub edx, edx");
        addFunction("        mov ecx, 10");
        addFunction("        div ecx");
        addFunction("        add dl, '0'");
        addFunction("        mov [edi], dl");
        addFunction("        dec edi");
        addFunction("        test eax, eax");
        addFunction("        jnz itoa_loop");
        addFunction("    leave");
        addFunction("    ret");
        addFunction("");
    }
}

inline void Translator::addWarning( string text )
{
    m_Warnings += "- " + text + "\n";
}

void Translator::addMemory( int add, string value )
{
    m_Memory[add] = value;
    m_MemoryIA32[add] = "var" + to_string(add);
}

void Translator::addMemoryLabel( int address )
{
    // Verifica se a label ja existe e adiciona se for nova
    if( std::find(m_Labels.begin(), m_Labels.end(), address) == m_Labels.end() )
    {
        m_Labels.push_back(address);
    }
}

inline string Translator::getMemoryValue( string add )
{
    return m_Memory[stoi(add)];
}

int Translator::getAcc()
{
    return m_Accumulator;
}

inline void Translator::setAcc( string value )
{
    m_Accumulator = stoi(value);
}

inline bool Translator::hasStopped() 
{ 
    return m_hasStopped; 
}

inline void Translator::setStopped( bool value )
{ 
    m_hasStopped = value; 
}

inline void Translator::setHasInput( bool value )
{
    m_hasInput = value;
}


inline void Translator::setHasOutput( bool value )
{
    m_hasOutput = value;   
}

// Mostra a memoria na tela
void Translator::showMemory()
{
    cout << "Espacos para alocar:" << endl;
    for( auto it = m_Memory.begin(); it != m_Memory.end(); ++it)
    {
        cout << " Endereco: " << it->first << " | Valor: " << it->second << endl;
    }
    cout << endl;
    
    cout << "Memoria:" << endl;
    for( auto it = m_MemoryIA32.begin(); it != m_MemoryIA32.end(); ++it)
    {
        cout << " Endereco: " << it->first << " | Valor: " << it->second << endl;
    }
    cout << endl;

    cout << "Labels inferidas:" << endl << " ";
    for( int n : m_Labels )
    {
        cout << n << " - ";
    }
}

// Adiciona uma entrada nos logs
void Translator::addLog( string value )
{
    m_Log += " - " + value + "\n";
}

// Verifica as variaveis e cria a variavel no codigo IA32
void Translator::createVariables()
{
    for( auto it = m_Memory.begin(); it != m_Memory.end(); ++it)
    {
        if(it->second == "0")
        {
            // // Se for um input
            // if(inputs.find(it->first) != inputs.end())
            // {
            //     addBSS( m_MemoryIA32[it->first], "dd", inputs[it->first] );
            // }
            // Se for um espaco de memoria
            if(m_MemoryIA32.find(it->first) != m_MemoryIA32.end())
            {
                addBSS( m_MemoryIA32[it->first] );
            }
        }        
        else
        {
            // Se for uma constante
            if(m_MemoryIA32.find(it->first) != m_MemoryIA32.end())
            {
                addEqu( m_MemoryIA32[it->first], it->second );
            }
        }
    }
}

// Inicializa com o codigo base
void Translator::initializateCode()
{
    m_IA32_data = "section .data\n";
    m_IA32_bss  = "section .bss\n";
    m_IA32_text = "section .text\n";

    // Inicializa o Acumulador em memória
    addData("ACC", "dd", "0");

    // Inicializa o start
    addText("global _start\n");
    addLabel("_start");
} 

// Retorna o codigo em IA32 final
inline string Translator::getCode()
{
    return m_IA32_data + "\n" + m_IA32_bss + "\n" + m_IA32_text + "\n" + m_IA32_text_functions;
}

inline string Translator::getWarnings()
{
    return m_Warnings;
}

inline string Translator::getLog()
{
    return m_Log;
}

inline vector<int> Translator::getLabels()
{
    return m_Labels;
}

/********************************
    OPCODES
*********************************
IA32:
    ADD DST, SRC
    ------------------------
    SRC + DST -> DST

Assembly:
    [valor ja no ACC]
    ADD  SRC
    ------------------------
    SRC + ACC -> ACC
*/

inline void Translator::ADD( string value )
{
    int result = stoi(value) + getAcc();

    setAcc(to_string(result));

    addText("add [ACC], [" + m_MemoryIA32[stoi(value)] + "]");
}
inline void Translator::SUB( string value)
{
    int result = stoi(value) - getAcc();

    setAcc(to_string(result));

    addText("sub [ACC], [" + m_MemoryIA32[stoi(value)] + "]");
}
// retorna o status do Overflow
string Translator::MUL( string value )
{
    string status = "OK";

    // Checa se houve overflow na multiplicacao.
    addLog("[MUL] Value: " + value + " | " + "ACC: " + to_string(getAcc()));
    // cin.get();
    int a = stoi(value);
    int b = getAcc();

    long test = static_cast<long>(a) * static_cast<long>(b);
    if( test > std::numeric_limits<int>::max() || test < std::numeric_limits<int>::min() )
    {
        status = "Overflow";
    }

    addText("mov eax, [ACC]");
    addText("mov ebx, [" + m_MemoryIA32[stoi(value)] + "]");
    addText("imul ebx");
    addText("mov [ACC], eax");
    addText("");


    return status;
}
void Translator::DIV( string value)
{
    addText("mov ecx, [ACC]");
    addText("mov eax, [" + m_MemoryIA32[stoi(value)] + "]");
    addText("cdq"); // extensao de sinal
    addText("idiv ebx");
    addText("mov [ACC], eax");
    addText("");
}
void Translator::INPUT( string value )
{
    addText("mov eax, 3");
    addText("mov ebx, 1");
    addText("mov ecx, var" + value);
    addText("mov edx, 32");
    addText("int 0x80");
    addText("");
    addText("push var" + value);
    addText("call atoi");
    addText("mov [var" + value + "], eax" );
    addText("");
}
void Translator::OUTPUT( string value )
{
    addText("mov edi, buffer + 30");
    addText("mov eax, [var"+ value + "]");
    addText("call itoa");
    addText("");
    addText("mov eax, buffer + 31");
    addText("sub eax, edi");
    addText("mov edx, eax");
    addText("mov eax, 4");
    addText("mov ebx, 1");
    addText("lea ecx, [edi + 1]");
    addText("int 0x80");
    addText("");

}
inline void Translator::COPY( string dst, string src)
{
    addText("mov [" + m_MemoryIA32[stoi(dst)] + "], [" + m_MemoryIA32[stoi(src)] + "]");
    addText("");
}
inline void Translator::STORE( string value )
{
    addText("mov [" + m_MemoryIA32[stoi(value)] + "], [ACC]");
    addText("");
}
inline void Translator::LOAD( string value )
{
    setAcc(value);
    addText("mov [ACC], [" + m_MemoryIA32[stoi(value)] + "]");
    addText("");
}
/*
    JUMPS

    Compara com o ACC para saber o que fazer

    JMP -> pulo direto
    JMPN (jl) -> se ACC < 0,  entao pula
    JMPP (jg) -> se ACC > 0,  entao pula
    JMPZ (je) -> se ACC == 0, entao pula

    Eh preciso comparar (cmp) antes de realizar o pulo.
*/
inline void Translator::JMP( string value )
{
    addText("jmp LABEL" + value);
    addText("");
}
void Translator::JMPN( string value )
{
    addText("cmp [ACC], 0");
    addText("jl LABEL" + value);
    addText("");
}
void Translator::JMPP( string value )
{
    addText("cmp [ACC], 0");
    addText("jg LABEL" + value);
    addText("");
}
void Translator::JMPZ( string value )
{
    addText("cmp [ACC], 0");
    addText("je LABEL" + value);
    addText("");
}
void Translator::STOP()
{
    addText("mov eax, 1");
    addText("xor ebx, ebx");
    addText("int 0x80");

    m_hasStopped = true;
}

/********************************
    Funcoes ajudantes
*********************************/
int getMemorySpace( string instruction ) 
{
    if( instruction == "COPY" )
        return 3;

    if( instruction == "STOP" || instruction == "CONST" )
        return 1;

    if( instruction == "SPACE" )
        return 1;

    if( instruction == "BEGIN" || instruction == "SECTION" || instruction == "END" || instruction == "EXTERN" || instruction == "PUBLIC" )
        return 0;

    return 2;    
}

int getMemorySpace( unsigned int instruction )
{
    if (instruction == 9)
        return 3;

    if (instruction == 14 )
        return 1;

    if (instruction == 0)
        return 0;

    return 2;
}


#endif