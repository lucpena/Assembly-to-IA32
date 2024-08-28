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
using std::map;
using std::vector;
using std::pair;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::toupper;
using std::stoi;
using std::to_string;

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

// Variaveis do Tradutor
vector<string>              program_source = {};
map<uint32_t, string>       program_IA32   = {};

class Tradutor
{
private:



public:

    Tradutor() {}
    ~Tradutor() {};

    bool AssemblyToIA32();

};

bool Tradutor::AssemblyToIA32()
{

    return true;
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