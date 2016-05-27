#include "dungeon_data.h"
#include <sstream>
#include "string_constants.h"
#include "utils.h"

using namespace std;

void DungeonEntity::setName(string name)
{
	_name = name;
	_lcasename = toLower(name);
}

string DungeonEntity::getName() const
{
	return _name;
}

string DungeonEntity::getLcaseName() const
{
	return _lcasename;
}


DungeonRoom::DungeonRoom()
{
	
}

DungeonRoom::~DungeonRoom()
{

}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"uid\":\"" << uid << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"name\":\"" << getName() << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"description\":\"" << join(0,description,"\n") << "\"," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]" << endl;
	return sout.str();
}

DungeonExit::DungeonExit()
{	
	isDoor = false;
	isOpen = false; //has no meaning when not a door
	openText ="is open";
	closedText = "is closed";
	distance = 1;

}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;

	sout << "\"name\":\"" << getName() << "\", \"description\":\"" << join(0,description,"\n")
		 << "\", \"links\":" << room->uid;

	return sout.str();
}

DungeonObject::DungeonObject()
{

	damage = 0;
	mass =0;
	size=0;

	canOpen = false;
	isOpen = false;
	canTake = true;


	selfEffect = USE_EFFECT::NONE;
	creatureEffect = USE_EFFECT::NONE;
	objectEffect = USE_EFFECT::NONE;

}

DungeonObject::~DungeonObject()
{

}
string DungeonObject::toJSON()
{
	ostringstream sout;
	sout << "\"name\":\"" << getName() << "\", \"description\":\"" << join(0,description,"\n") << "\",";
	sout << " \"damage\":" << damage << ",\"mass\":" << mass << ", \"size\":" << size << ",";
	sout << " \"canOpen\":" << canOpen << ",\"canTake\":" << canTake << ", \"isOpen\":" << isOpen;

	return sout.str();
}

DungeonCreature::DungeonCreature()
{
	hitpoints = 100;
}
string DungeonCreature::attack(DungeonObject *weapon,DungeonPlayer *player)
{
	hitpoints = hitpoints - weapon->damage;
	if(hitpoints <= 0) return "You have killed the "+getName()+"!";
	return "You hit the " + getName();
}
DungeonCreature::~DungeonCreature()
{
}
string DungeonCreature::toJSON()
{
	ostringstream sout;

	sout << "\"name\":\"" << getName() << "\", \"description\":\"" << join(0,description,"\n")
		 << "\", \"hitpoints\":" << hitpoints << ", \"alignment\":" << alignment;
	return sout.str();
}

DungeonPlayer::DungeonPlayer()
{

}

DungeonPlayer::~DungeonPlayer()
{

}
