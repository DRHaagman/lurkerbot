#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <stdlib.h>

#include "libircclient.h"
#include "json.h"

using namespace std;

namespace TWDevNet {
	typedef struct {
		string name;
		string alias;
		string command;
		string type;
	} irc_chan_act_t;

	typedef struct {
		string name;
		string flags;
		string actiontrigger;
		irc_chan_act_t *actions[255];
	} irc_chan_t;

	typedef struct {
		string realname;
		string name;
		string type;
		string server;
		unsigned short port;
		string username;
		string password;
		string nick;
		string nickpass;
		irc_chan_t *channels[255];
	} irc_ctx_t;

	void addlog(const char * fmt, ...);
}
#endif // INCLUDE_MAIN_H