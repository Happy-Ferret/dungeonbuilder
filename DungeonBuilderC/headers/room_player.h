
#ifndef ROOM_PLAYER
#define ROOM_PLAYER

#include "headers/curses.h"
#include "headers/dungeon_data.h"
#include "headers/printutils.h"
#include "headers/utils.h"
#include "headers/command_window.h"
#include <map>

extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;

struct RoomPlayer
{
	typedef string (RoomPlayer::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonRoom *room;
	WINDOW *headerWindow;
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room);

	
	string exit(vector<string> args);
	
};




#endif
