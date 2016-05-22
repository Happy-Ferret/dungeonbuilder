#include "headers/main_menu.h"

using namespace std;

DungeonRoom *g_startRoom;
vector<DungeonRoom*> g_roomList;


void MainMenu::exitMenu(vector<string> args)
{
	exit(EXIT_SUCCESS);
}

void MainMenu::create(vector<string> args)
{
	//Creating a new game
	//Create a room editor
	//Create a start room and set the global start room pointer
	//Add the room to the global room list
	//Edit the room
	RoomEditor ed;
	g_startRoom = new DungeonRoom();
	g_roomList.push_back(g_startRoom);
	g_startRoom->name = "Default Name";
	g_startRoom->description = "Default Description";
	clearWindows();
	ed.load(g_startRoom);
	resetWindows();
}

void MainMenu::play(vector<string> args)
{
	RoomPlayer rp;
	rp.load(g_startRoom);
	resetWindows();
}

void MainMenu::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void MainMenu::resetWindows()
{
	clear();
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS-8,0,4);
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

	int done = false;
	string command;

	setcolor(mainWindow,1,COLOR_RED);
	mvwprintwCenterBold(mainWindow,3,"Dungeon Builder");
	setcolor(mainWindow,2,COLOR_CYAN);
	mvwprintwBold(mainWindow,5,0,"[Create] a Dungeon");
	mvwprintwBold(mainWindow,6,0,"[Enter] Dungeon");
	mvwprintwBold(mainWindow,7,0,"[Exit] this world");
	wrefresh(mainWindow);


}

void MainMenu::load()
{
	cmdMap["create"] = &MainMenu::create;
	cmdMap["exit"] = &MainMenu::exitMenu;
	cmdMap["enter"] = &MainMenu::play;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,":");
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
		else {
			commandFunction cmdFunc = cmdMap[cmd[0]];
			(this->*cmdFunc)(cmd);
		}
	}



	clearWindows();
}





