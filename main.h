#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "libircclient.h"
#include "json.h"

namespace TWDevNet {
	typedef struct {
		char *name;
		char *alias;
		char *command;
		char *type;
	} irc_chan_act_t;

	typedef struct {
		char *name;
		char *flags;
		char *actiontrigger;
		irc_chan_act_t *actions[255];
	} irc_chan_t;

	typedef struct {
		char *realname;
		char *name;
		char *type;
		char *server;
		unsigned short port;
		char *username;
		char *password;
		char *nick;
		char *nickpass;
		irc_chan_t *channels[255];
	} irc_ctx_t;

	void addlog(const char * fmt, ...);
}
#endif // INCLUDE_MAIN_H