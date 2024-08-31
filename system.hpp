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
vector<string>  program_source  = {};

class Translator
{
private:

    string m_IA32_data;
    string m_IA32_bss;
    string m_IA32_text;
    string m_Accumulator;
    string m_Warnings;
    string m_Log;

    uint32_t m_AllocatedMemory;

    map<int, string> m_Memory;
    map<int, string> m_MemoryIA32;
    vector<int>      m_Labels;

    bool m_hasStopped;

public:

    Translator():         
        m_IA32_data(""), 
        m_IA32_bss(""), 
        m_IA32_text(""),
        m_Accumulator(""),
        m_Warnings(""),
        m_Log(""),
        m_AllocatedMemory(0),
        m_Memory({}),
        m_MemoryIA32({}),
        m_Labels({}),
        m_hasStopped(false) {}

    ~Translator() {};

    void initializateCode();
    void createVariables();
    void addMemory( int add, string value );

    // inline string getAccumulator() { return m_Accumulator;}
    // inline void setAccumulator( string value ) { m_Accumulator = value; }

    void ADD( string value );
    void SUB( string value );
    void MUL( string value );
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

    void addData( string data );
    void addBSS( string bss );
    void addText( string text );
    void addLabel( string label );
    void addMemoryLabel( int add );
    void addEqu( string data, string value );

    string getMemoryValue( string add );

    void addWarning( string value );
    void addLog( string value );
    void showMemory();

    string getLog();
    string getCode();
    string getWarnings();
    vector<int> getLabels(); 
    
    inline bool hasStopped() { return m_hasStopped; };
    inline void setStopped( bool value ) { m_hasStopped = value; };

};
/********************************
    Definicoes do Tradutor
*********************************/

// Funcoes de traducao
inline void Translator::addLabel( string label )
{
    m_IA32_text += label + ":\n";
}

inline void Translator::addData( string data )
{
    m_IA32_data += "    " + data + " dd 0\n";
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

inline void Translator::addWarning( string text )
{
    m_Warnings += "- " + text + "\n";
}

inline void Translator::addMemory( int add, string value )
{
    m_Memory[add] = value;
    m_MemoryIA32[add] = "var" + to_string(add);
}

inline void Translator::addMemoryLabel( int address )
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
        if(it->second =="0")
        {
            if(m_MemoryIA32.find(it->first) != m_MemoryIA32.end())
            {
                addBSS( m_MemoryIA32[it->first]);
            }
        }
        else{
            if(m_MemoryIA32.find(it->first) != m_MemoryIA32.end())
            {
                addEqu(m_MemoryIA32[it->first], it->second);
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
    addData("ACC");

    // Inicializa o start
    addText("global _start\n");
    addLabel("_start");
} 

// Retorna o codigo em IA32 final
inline string Translator::getCode()
{
    return m_IA32_data + "\n" + m_IA32_bss + "\n" + m_IA32_text;
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

inline void Translator::ADD( string value)
{
    addText("add [ACC], []" + m_MemoryIA32[stoi(value)] + "]");
}
inline void Translator::SUB( string value)
{
    addText("sub [ACC], [" + m_MemoryIA32[stoi(value)] + "]");
}
inline void Translator::MUL( string value)
{
    addText("mov eax, [ACC]");
    addText("mov ebx, [" + m_MemoryIA32[stoi(value)] + "]");
    addText("imul ebx");
    addText("mov [ACC], eax");
}
inline void Translator::DIV( string value)
{
    addText("mov ecx, [ACC]");
    addText("mov eax, [" + m_MemoryIA32[stoi(value)] + "]");
    addText("cdq"); // extensao de sinal
    addText("idiv ebx");
    addText("mov [ACC], eax");
}
inline void Translator::INPUT( string value )
{
    addText("mov [ACC], [" + m_MemoryIA32[stoi(value)] + "]");
}
inline void Translator::OUTPUT( string value )
{
    addText("OUTPUT [" + m_MemoryIA32[stoi(value)] + "]");
}
inline void Translator::COPY( string dst, string src)
{
    addText("mov [" + m_MemoryIA32[stoi(dst)] + "], [" + m_MemoryIA32[stoi(src)] + "]");
}
inline void Translator::STORE( string value )
{
    addText("mov " + m_MemoryIA32[stoi(value)] + ", [ACC]");
}
inline void Translator::LOAD( string value )
{
    addText("mov [ACC], " + value);
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
}
inline void Translator::JMPN( string value )
{
    addText("cmp [ACC], 0");
    addText("jl LABEL" + value);
}
inline void Translator::JMPP( string value )
{
    addText("cmp [ACC], 0");
    addText("jg LABEL" + value);
}
inline void Translator::JMPZ( string value )
{
    addText("cmp [ACC], 0");
    addText("je LABEL" + value);
}
inline void Translator::STOP()
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