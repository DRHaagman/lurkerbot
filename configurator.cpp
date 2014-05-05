#include <string>
#include "main.h"
#include "configurator.h"

using namespace std;

namespace TWDevNet {
	Configurator::Configurator() {
		servercount = 0;
		valid = false;
	}
	Configurator::Configurator(const char *filename) {
		Configurator();
		valid = ReadConfig(filename);
	}
	Configurator::~Configurator() {
	}

	string Configurator::FetchAndUnquoteJSElement(json_object *jsobject, string ElementName) {
		json_object *jsonTemp;
		json_object_object_get_ex(jsobject, ElementName.c_str(), &jsonTemp);
		string strTemp = json_object_to_json_string(jsonTemp);
		return strTemp.substr(1, strTemp.length() - 2);
	}
	int Configurator::FetchIntJSElement(json_object *jsobject, string ElementName) {
		json_object *jsonTemp;
		json_object_object_get_ex(jsobject, ElementName.c_str(), &jsonTemp);
		return json_object_get_int(jsonTemp);
	}
	int Configurator::ReadConfig(const char *filename) {
		string strTemp;
		// Read the config file
		// Parse out the JSON and apply each server to an array of context structs.
		json_object* jsonconfig = json_object_from_file(filename);
		addlog("Parsing the configuration file");
		if (!jsonconfig) {
			addlog("Configuration JSON format invalid.");
			return false;
		}
		else {
			addlog("Configuration JSON format valid.");
//			addlog("%s", (char*)json_object_to_json_string_ext(jsonconfig, JSON_C_TO_STRING_PRETTY));
			addlog("%d elements", json_object_object_length(jsonconfig));

			json_object *jsonbotname;
			if (json_object_object_get_ex(jsonconfig, "name", &jsonbotname)) {
				strTemp = FetchAndUnquoteJSElement(jsonconfig, "name");
				addlog("Bot Name: %s", strTemp.c_str());
			}
			json_object *jsonservers;
			json_object *jschannels;
			json_object *jsactions;
			json_object *jsonserver; // Used repeatedly for each server in the list.
			json_object *jschannel;  // Used repeatedly for each channel in the list.
			json_object *jsaction;   // Used repeatedly for each action in the list.
			json_object_object_get_ex(jsonconfig, "servers", &jsonservers);
			const int ServerCount = json_object_array_length(jsonservers);
			addlog("Server Array Length: %d", ServerCount);
			this->servercount = ServerCount;
			for (int x = 0;x < ServerCount;x++) {
				servers[x].sessionno = x;
				servers[x].realname = strTemp.c_str(); //substr(botname, 1, strlen(botname)-2);
				jsonserver = json_object_array_get_idx(jsonservers, x);
				servers[x].name = FetchAndUnquoteJSElement(jsonserver, "name");
				servers[x].type = FetchAndUnquoteJSElement(jsonserver, "type");
				servers[x].server = FetchAndUnquoteJSElement(jsonserver, "host");
				servers[x].port = FetchIntJSElement(jsonserver, "port");
				servers[x].username = FetchAndUnquoteJSElement(jsonserver, "username");
				servers[x].password = FetchAndUnquoteJSElement(jsonserver, "password");
				servers[x].nick = FetchAndUnquoteJSElement(jsonserver, "nick");
				servers[x].nickpass = FetchAndUnquoteJSElement(jsonserver, "identify");

				if (json_object_object_get_ex(jsonserver, "channels", &jschannels)) {
					int ChannelCount = json_object_array_length(jschannels);
					servers[x].channelcount = ChannelCount;
					addlog("Server Channels Array Length: %d", ChannelCount);
					for (int y = 0;y < ChannelCount;y++) {
						irc_chan_t *channel = new irc_chan_t;
						servers[x].channels[y] = channel;
						jschannel = json_object_array_get_idx(jschannels, y);
						servers[x].channels[y]->name = FetchAndUnquoteJSElement(jschannel, "name");
						servers[x].channels[y]->flags = FetchAndUnquoteJSElement(jschannel, "flags");
						servers[x].channels[y]->actiontrigger = FetchAndUnquoteJSElement(jschannel, "actiontrigger");

						if (json_object_object_get_ex(jschannel, "actions", &jsactions)) {
							int ActionCount = json_object_array_length(jsactions);
							servers[x].channels[y]->actioncount = ActionCount;
							addlog("Server Channel Actions Array Length: %d", ActionCount);
							for (int z = 0;z < ActionCount;z++) {
								irc_chan_act_t *action = new irc_chan_act_t;
								servers[x].channels[y]->actions[z] = action;
								jsaction = json_object_array_get_idx(jsactions, z);
								servers[x].channels[y]->actions[z]->name = FetchAndUnquoteJSElement(jsaction, "name");
								servers[x].channels[y]->actions[z]->alias = FetchAndUnquoteJSElement(jsaction, "alias");
								servers[x].channels[y]->actions[z]->command = FetchAndUnquoteJSElement(jsaction, "command");
								servers[x].channels[y]->actions[z]->type = FetchAndUnquoteJSElement(jsaction, "type");
							}
						}
					}
				}
			}
		}
		return true;
	}
}