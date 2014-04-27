#ifndef INCLUDE_TWDIRC_H
#define INCLUDE_TWDIRC_H
#include <iostream>
#include <string>

#include "libircclient.h"

namespace TWDevNet {

	class IRC {
		public:
			irc_callbacks_t callbacks;
			irc_session_t * s;

		public:
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

		public:
			irc_ctx_t *ctx;

		public:
			IRC();
			~IRC();

		public:
			irc_session_t * CreateSession(irc_callbacks_t * callbacks);
			void DestroySession(irc_session_t * session);
			int Connect();
			int Run();
			void dump_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void notice_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
			void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count);
			void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
			void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
			void irc_event_dcc_chat(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid);
			void irc_event_dcc_send(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid);
	};
}

#endif /* INCLUDE_TWDIRC_H */