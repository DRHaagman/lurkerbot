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
		char *server;
		unsigned short port;
		char *channel;
		char *nick;
		char *nickpass;
		char *password;
		char *username;
		char *realname;
	} irc_ctx_t;

	void addlog(const char * fmt, ...);
}
#endif // INCLUDE_MAIN_H