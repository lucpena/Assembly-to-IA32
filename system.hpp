#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

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

#endif