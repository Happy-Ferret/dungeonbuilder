#include "dungeon_room.h"
#include "dungeon_exit.h"
#include "exit_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"
#include "room_list.h"

using namespace std;

string ExitEditor::exit(vector<string> args)
{
	return STR_EXIT;
}


string ExitEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command";
	}
	string setNoun = args[1];
	toLower(&setNoun);

	if(setNoun == STR_ROOM)
	{
		DungeonRoomList dl;
		DungeonRoom * newRoom = dl.load(globalState.roomList,dungeonExit);
		dungeonExit->room = newRoom;

	}
	else if(setNoun ==STR_NAME)
	{
		string newname = join(2,args,CHR_SPACE);
		dungeonExit->setPrimaryName(newname);
	}
	else if(setNoun == STR_DOOR)
	{
		dungeonExit->isDoor = isAffirmative(args[2]);
	}
	else if(setNoun == STR_OPEN)
	{
		dungeonExit->isOpen = isAffirmative(args[2]);
	}
	else if(setNoun == STR_OPEN_TEXT || setNoun == STR_DESC)
	{
		dungeonExit->openText = join(2,args,CHR_SPACE);
	}
	else if(setNoun == STR_CLOSED_TEXT)
	{
		dungeonExit->closedText = join(2,args,CHR_SPACE);
	}
	else if(setNoun == STR_CLOSING_TEXT)
	{
		dungeonExit->closingText = join(2,args,CHR_SPACE);
	}
	else if(setNoun == STR_OPENING_TEXT)
	{
		dungeonExit->openingText = join(2,args,CHR_SPACE);
	}
	else if(setNoun == STR_TRAVEL_TEXT)
	{
		dungeonExit->travelText = join(2,args,CHR_SPACE);
	} else if (setNoun == STR_MIRROR)	
	{
		if (isAffirmative(args[2]) && !isExitMirrored())
		{
			dungeonExit->mirror();
		} 
		else if (!isAffirmative(args[2]))
		{
			DungeonExit* toDelete = nullptr;
			for(auto r : dungeonExit->room->exits)
			{
				if(r->room == dungeonExit->parent)
				{
					string dir = toLower(DungeonExit::getOppositeDirection(r->getPrimaryName()));
					if(dir == toLower(dungeonExit->getPrimaryName()))
					{
						toDelete = r;
						break;
					}
				}
			}
			
			delete toDelete;
			
		}
	}



	else {
		return "I don't know how to set that";
	}
	clearWindows();
	resetWindows();
	return "";
}

string ExitEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);

	if(delNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to delete.";
		}
		string name = join(2,args,CHR_SPACE);
		if(!dungeonExit->removeName(name)) {
			return "You can't.";
		}

	}
	clearWindows();
	resetWindows();
	return "";
}


string ExitEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);

	if(addNoun == STR_NAME)
	{
		if(args.size() < 3)
		{
			return "Provide a name to add please.";
		}
		string name = join(2,args,CHR_SPACE);
		dungeonExit->addName(name);
	}
	else
	{
		return "I don't know how to add that";
	}
	clearWindows();
	resetWindows();
	return "";
}


//Checks if this exit, has a mirror exit going
//The other way
bool ExitEditor::isExitMirrored()
{	
	for (auto r : dungeonExit->room->exits)
	{
		if (r->room == dungeonExit->parent)
		{
			string dir = toLower(DungeonExit::getOppositeDirection(r->getPrimaryName()));
			if (dir == toLower(dungeonExit->getPrimaryName()))
				return true;
		}
	}
	return false;

}


string ExitEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == STR_NAME)
	{
		return set(args);

	}
	else if(editNoun == STR_ROOM)
	{
		return STR_ROOM;
	}
	else
	{
		return "I don't know how to edit that";
	}

}


void ExitEditor::clearWindows()
{
	delwin(headerWindow);
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
}

void ExitEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	headerWindow = newwin(1,COLS,0,0);
	mainWindow = newwin(LINES-3,COLS,1,0);
	textBuffer.clear();
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);



	printHeader(headerWindow,"Exit",dungeonExit->parent->getPrimaryName(),dungeonExit->getPrimaryName(),dungeonExit->room->getPrimaryName());
	
	string nameRow = STR_MENU_NAME + join(0,dungeonExit->getNames(),',');
	textBuffer.push_back(nameRow);

	if (dungeonExit->room != dungeonExit->parent)
	{
		string isMirror = isExitMirrored() ? "T" : "F";
		string mirrorRow = STR_MENU_MIRROR + isMirror;
		textBuffer.push_back(mirrorRow);
	}
	
	string travelRow = STR_MENU_TRAVEL_TEXT +  dungeonExit->travelText;
	textBuffer.push_back(travelRow);
	
	string roomRow = STR_MENU_ROOM + dungeonExit->room->getPrimaryName();
	textBuffer.push_back(roomRow);


	
	string torf = dungeonExit->isDoor ? STR_TRUE : STR_FALSE;
	string doorRow = STR_MENU_IS_DOOR + torf;
	textBuffer.push_back(doorRow);

	if(dungeonExit->isDoor) {

		torf = dungeonExit->isOpen ? STR_TRUE : STR_FALSE;
		string openRow = STR_MENU_IS_OPEN + torf;
		textBuffer.push_back(openRow);
		
		string openTextRow = STR_MENU_OPENTEXT + dungeonExit->openText;
		textBuffer.push_back(openTextRow);
		
		string closedTextRow = STR_MENU_CLOSEDTEXT + dungeonExit->closedText;
		textBuffer.push_back(closedTextRow);
		
		string openingTextRow = STR_MENU_OPENINGTEXT + dungeonExit->openingText;
		textBuffer.push_back(openingTextRow);
		
		string closingTextRow = STR_MENU_CLOSINGTEXT + dungeonExit->closingText;
		textBuffer.push_back(closingTextRow);

	}
	renderTextBuffer();
	wrefresh(mainWindow);

}

DungeonRoom* ExitEditor::load(DungeonExit *_dungeonExit)
{	

	dungeonExit = _dungeonExit;
	

	cmdMap[STR_EDIT] = &ExitEditor::edit;
	cmdMap[STR_EXIT] = &ExitEditor::exit;
	cmdMap[STR_SET] = &ExitEditor::set;
	cmdMap[STR_ADD] = &ExitEditor::add;
	cmdMap[STR_DELETE] = &ExitEditor::del;


	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;

	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			if (checkCommonInput(cmd[0])) continue;
			cmdFound = cmdMap.count(cmd[0]) > 0;
		}

		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
		else
		{
			commandFunction cmdFunc = cmdMap[cmd[0]];
			string response = (this->*cmdFunc)(cmd);
			if(response == STR_ROOM)
			{
				return dungeonExit->room;
			}
			else if(response == STR_EXIT)
			{
				return nullptr;
			}

			if(response.length() > 0) {
				cmd.clear();
				mvwprintw(responseWindow,0,0,response.c_str());
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
		}
	}
}





