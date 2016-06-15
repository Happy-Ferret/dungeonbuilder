#include "dungeon_serializer.h"
#include "string_constants.h"
#include "dungeon_room.h"
#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_effect.h"
#include "dungeon_action.h"
#include "dungeon_trigger.h"
#include "dungeon_creature.h"
#include "json.h"
using namespace std;



extern DungeonRoom *g_startRoom;
extern DungeonPlayer *g_player;
extern vector<DungeonRoom*> g_roomList;
extern vector<DungeonObject*> g_objectList;
extern vector<DungeonCreature*> g_creatureList;
extern vector<DungeonExit*> g_exitList;
extern vector<DungeonAction*> g_actionList;
extern vector<DungeonEffect*> g_effectList;
extern vector<DungeonTrigger*> g_triggerList;


int _loadInt(string name,void *_json)
{
	json_value * json = (json_value*)_json;
	for(int i = 0; i < json->u.object.length; i++)
	{
		string json_name = json->u.object.values[i].name;
		if(json_name == name)
		{
			if(json->u.object.values[i].value->type == json_integer)
				return json->u.object.values[i].value->u.integer;
			else
				break;
		}
	}
	return 0; //todo - proper error here?
}

string _loadString(string name,void *_json)
{
	json_value * json = (json_value*)_json;
	for(int i = 0; i < json->u.object.length; i++)
	{
		string json_name = json->u.object.values[i].name;
		if(json_name == name)
		{
			if(json->u.object.values[i].value->type == json_string)
				return json->u.object.values[i].value->u.string.ptr;
			else
				break;
		}
	}
	return ""; //todo - proper error here?
}

bool _loadBool(string name,void *_json)
{
	json_value * json = (json_value*)_json;
	for(int i = 0; i < json->u.object.length; i++)
	{
		string json_name = json->u.object.values[i].name;
		if(json_name == name)
		{
			if(json->u.object.values[i].value->type == json_boolean)
				return json->u.object.values[i].value->u.boolean;
			else
				break;
		}
	}
	return false; //todo - proper error here?
}

vector<string> _loadVectorString(string name,void *_json)
{
	json_value * json = (json_value*)_json;
	vector<string> result;
	for(int i = 0; i < json->u.object.length; i++)
	{
		string json_name = json->u.object.values[i].name;
		if(json_name == name)
		{
			if(json->u.object.values[i].value->type == json_array)
			{
				auto v = json->u.object.values[i].value->u.array;
				for(int j = 0; j < v.length; j++)
				{
					if(v.values[j]->type == json_string) {
						string s = v.values[j]->u.string.ptr;
						result.push_back(s);
					}
					else {
						break;
					}
				}
				break;
			}
			else
				break;
		}
	}
	return result; //todo - proper error here?
}

void _loadVectorEntity(string name,void *_v,void *_json)
{
	json_value * json = (json_value*)_json;
	vector<DungeonEntity*> *result = (vector<DungeonEntity*> *)_v;
	for(int i = 0; i < json->u.object.length; i++)
	{
		string json_name = json->u.object.values[i].name;
		if(json_name == name)
		{
			if(json->u.object.values[i].value->type == json_array)
			{
				auto v = json->u.object.values[i].value->u.array;
				for(int j = 0; j < v.length; j++)
				{
					if(v.values[j]->type == json_integer) {
						int uid = v.values[j]->u.integer;
						result->push_back((DungeonEntity*)uid);
					}
					else {
						break;
					}
				}
				break;
			}
			else
				break;
		}
	}	
}



string _writeInt(string name,int value)
{
	return STR_QUOT + name + STR_QUOT + ":" + to_string(value) +",";
}

string _writeString(string name,string value)
{
	return STR_QUOT + name +STR_QUOT+ ":" + STR_QUOT + value + STR_QUOT+",";
}

string _writeBool(string name,bool value)
{
	string truth;
	
	if (value) truth = "true";
	else truth = "false";

	return STR_QUOT + name +STR_QUOT + ":" +truth+",";
}

string _writeVectorString(string name,vector<string> const &value)
{
	string result =  STR_QUOT + name + STR_QUOT+ ":[";
	for(unsigned int i = 0; i < value.size(); i++)
	{
		result += STR_QUOT + value[i] + STR_QUOT;
		if(i != value.size()-1) {
			result += ",";
		}
	}
	result = result +"],";
	return result;
}

//This only writes out the ids
string _writeVectorEntity(string name,void *value)
{
	vector<DungeonEntity*> *entityValue = (vector<DungeonEntity*>*)value;
	string result =  STR_QUOT + name + STR_QUOT+ ":[";
	for(auto v : *entityValue) {
		result += to_string(v->uid)+ ",";
	};
	result = result +"],";
	return result;
}




void loadJson(string filename)
{
	ifstream fin;
	filename.append(".json");
	fin.open(filename.c_str());
	string contents((istreambuf_iterator<char>(fin)),istreambuf_iterator<char>());
	const char *jsonStr = contents.c_str();

	//shallow load of all entities
	json_value* root = json_parse(jsonStr,contents.size()+1);
	for(int i = 0; i < root->u.object.length;i++)
	{
		auto v = root->u.object.values[i].value->u.array;
		string name = root->u.object.values[i].name;
		if(name == "rooms")
		{

			for(int j = 0; j < v.length; j++)
			{
				DungeonRoom *room = new DungeonRoom(v.values[j]);
				if(j == 0) g_startRoom = room;
				g_roomList.push_back(room);
			}

		}
		else if(name == "exits")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonExit *exit = new DungeonExit(v.values[j]);				
				g_exitList.push_back(exit);
			}
		}
		else if(name == "creatures")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonCreature *creature = new DungeonCreature(v.values[j]);				
				g_creatureList.push_back(creature);
			}
		}
		else if(name == "objects")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonObject *object = new DungeonObject(v.values[j]);				
				g_objectList.push_back(object);
			}
		}
		else if(name == "effects")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonEffect *effect = new DungeonEffect(v.values[j]);				
				g_effectList.push_back(effect);
			}
		}
		else if(name == "actions")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonAction *action = new DungeonAction(v.values[j]);				
				g_actionList.push_back(action);
			}
		}
		else if(name == "triggers")
		{
			for(int j = 0; j < v.length; j++)
			{
				DungeonTrigger *trigger = new DungeonTrigger(v.values[j]);				
				g_triggerList.push_back(trigger);
			}
		}
	}

	for(auto r : g_roomList)
	{
		r->fixUpPointers();
	}
	

}