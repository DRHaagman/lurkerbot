#include <string>
#include "main.h"
#include "configurator.h"

namespace TWDevNet {
	Configurator::Configurator() {
	}
	Configurator::Configurator(const char *filename) {
		Configurator();
		this->ReadConfig(filename);
	}
	Configurator::~Configurator() {
	}

//		protected:
	int Configurator::ReadConfig(const char *filename) {
		// Read the config file
		// Parse out the JSON and apply each server to an array of context structs.
		json_object* jsonconfig = json_object_from_file(filename);
		addlog("Parsing the configuration file");
		if (!jsonconfig) {
			addlog("%s", "Configuration JSON format invalid.");
			return 1;
		}
		else {
			addlog("Configuration JSON format valid.");
			TWDevNet::addlog("%s", (char*)json_object_to_json_string_ext(jsonconfig, JSON_C_TO_STRING_PRETTY));
			addlog("%d elements", json_object_object_length(jsonconfig));

			char *botname;
			json_object *jsonbotname;
			if (json_object_object_get_ex(jsonconfig, "name", &jsonbotname)) {
				botname = (char*)json_object_to_json_string(jsonbotname);
				addlog("Bot Name: %s", botname);
			}
			json_object *jsonservers;
			json_object *jschannels;
			json_object *jsactions;
			json_object *jsonserver; // Used repeatedly for each server in the list.
			json_object *jschannel;  // Used repeatedly for each channel in the list.
			json_object *jsaction;   // Used repeatedly for each action in the list.
			json_object *jselement;  // Used repeatedly to fetch the singleton entries.
			json_object_object_get_ex(jsonconfig, "servers", &jsonservers);
			const int ServerCount = json_object_array_length(jsonservers);
			addlog("Server Array Length: %d", ServerCount);
			for (int x = 0;x < ServerCount;x++) {
				servers[x].realname = botname; //substr(botname, 1, strlen(botname)-2);
				jsonserver = json_object_array_get_idx(jsonservers, x);
				json_object_object_get_ex(jsonserver, "name", &jselement);
				servers[x].name = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "type", &jselement);
				servers[x].type = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "host", &jselement);
				servers[x].server = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "port", &jselement);
				servers[x].port = json_object_get_int(jselement);
				json_object_object_get_ex(jsonserver, "username", &jselement);
				servers[x].username = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "password", &jselement);
				servers[x].password = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "nick", &jselement);
				servers[x].nick = (char*)json_object_to_json_string(jselement);
				json_object_object_get_ex(jsonserver, "identify", &jselement);
				servers[x].nickpass = (char*)json_object_to_json_string(jselement);
				if (json_object_object_get_ex(jsonserver, "channels", &jschannels)) {

					int ChannelCount = json_object_array_length(jschannels);
					addlog("Server Channels Array Length: %d", ChannelCount);
					for (int y = 0;y < ChannelCount;y++) {
						irc_chan_t *channel = new irc_chan_t;
						servers[x].channels[y] = channel;
						jschannel = json_object_array_get_idx(jschannels, y);
						json_object_object_get_ex(jschannel, "name", &jselement);
						servers[x].channels[y]->name = (char*)json_object_to_json_string(jselement);
						json_object_object_get_ex(jschannel, "flags", &jselement);
						servers[x].channels[y]->flags = (char*)json_object_to_json_string(jselement);
						json_object_object_get_ex(jschannel, "actiontrigger", &jselement);
						servers[x].channels[y]->actiontrigger = (char*)json_object_to_json_string(jselement);
						if (json_object_object_get_ex(jschannel, "actions", &jsactions)) {

							int ActionCount = json_object_array_length(jsactions);
							addlog("Server Channel Actions Array Length: %d", ActionCount);
							for (int z = 0;z < ActionCount;z++) {
								irc_chan_act_t *action = new irc_chan_act_t;
								servers[x].channels[y]->actions[z] = action;
								jsaction = json_object_array_get_idx(jsactions, z);
								json_object_object_get_ex(jsaction, "name", &jselement);
								servers[x].channels[y]->actions[x]->name = (char*)json_object_to_json_string(jselement);
								json_object_object_get_ex(jsaction, "alias", &jselement);
								servers[x].channels[y]->actions[x]->alias = (char*)json_object_to_json_string(jselement);
								json_object_object_get_ex(jsaction, "command", &jselement);
								servers[x].channels[y]->actions[x]->command = (char*)json_object_to_json_string(jselement);
								json_object_object_get_ex(jsaction, "type", &jselement);
								servers[x].channels[y]->actions[x]->type = (char*)json_object_to_json_string(jselement);
							}
						}
					}
				}
			}
/*
		servers[0].server = (char*)"irc.freenode.net";
		servers[0].port = 6667;
		servers[0].channel = (char*)"##planetminion";//"#twdev.bot"; //argv[3];
		servers[0].nick = (char*)"_lurker_"; //argv[2];
		servers[0].password = (char*)"";
		servers[0].username = (char*)"lurker";
		servers[0].nickpass = (char*)"********";
*/

		}
		return 0;
	}
}