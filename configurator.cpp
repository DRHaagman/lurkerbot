#include "configurator.h"

namespace TWDevNet {
	Configurator::Configurator() {
	}
	Configurator::Configurator(const char *filename) {
		this->ReadConfig(filename);
	}
	Configurator::~Configurator() {
	}

//		protected:
	void Configurator::ReadConfig(const char *filename) {
		// Read the config file
		// Parse out the JSON and apply each server to an array of context structs.
		json_object* config = json_object_from_file(filename);

		addlog("%s", json_object_to_json_string(config));

		servers[0].server = (char*)"irc.freenode.net";
		servers[0].port = 6667;
		servers[0].channel = (char*)"#twdev.bot"; //argv[3];
		servers[0].nick = (char*)"_lurker_"; //argv[2];
		servers[0].password = (char*)"";
		servers[0].username = (char*)"lurker";
		servers[0].realname = (char*)"Lurker Bot";
		servers[0].nickpass = (char*)"TvYxEw8M";
	}
}