#pragma once
#ifndef COMMAND_WINDOW
#define COMMAND_WINDOW

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif

#include <string>
#include <vector>
using namespace std;

struct CommandWindow
{
	WINDOW *window;
	int x;	
	int w;
	string input;
	int done = 0;
	string prompt;
				
	void reset();	
	vector<string> getCommand(WINDOW* _window,string prompt);				
	string getCommandAsString(WINDOW* _window,string prompt);				
	void moveLeft();
	void moveRight();			
	bool handleInput(int c);
	void print();
	

};


#endif