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
	// Public event functions
	void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void dump_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void notice_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count);
	void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
	void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
	void irc_event_dcc_chat(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid);
	void irc_event_dcc_send(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid);
}
#endif // INCLUDE_MAIN_H