#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "headers/printutils.h"
#include "headers/command_window.h"
#include "headers/utils.h"
#include "headers/dungeon_data.h"
#include "headers/room_editor.h"

using namespace std;
struct MainMenu
{
	typedef void (MainMenu::*commandFunction) (vector<string>);
	map<string,commandFunction> cmdMap;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	void create(vector<string> args);
	void exit(vector<string> args);
	void reset();
	void load();
	
};

#endif