#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include "dungeon_data.h"

using namespace std;


vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string join(unsigned int offset, vector<string> &v, string delim);
vector<string> removeArticles(vector<string> words);
bool containsWith(vector<string> word);
string toLower(string s);
void toLower(string *s);
bool isVowel(char c);
string a_an (string s);
bool isAffirmative(string s);
string thereIsA(string thing);
DungeonObject* extractObject(vector<DungeonObject*> *objects, string *userInput);
DungeonCreature* extractCreature(vector<DungeonCreature*> *objects, string *userInput);
DungeonObject* extractAndRemoveObject(vector<DungeonObject*> *objects, string *userInput);
DungeonCreature* extractAndRemoveCreature(vector<DungeonCreature*> *objects, string *userInput);

string extractPhrase(vector<string> *phrasesToFind, string *userInput);

#endif