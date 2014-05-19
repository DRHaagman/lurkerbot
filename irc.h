#ifndef INCLUDE_TWDIRC_H
#define INCLUDE_TWDIRC_H
#include <iostream>
#include <string>

#include "libircclient.h"

namespace TWDevNet {
	// Public event functions
	void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_dump(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_notice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_nick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_quit(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_part(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_mode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_topic(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_kick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_invite(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_umode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_ctcp_rep(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_ctcp_action(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_unknown(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
	void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count);
	void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
	void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
	void event_irc_dcc_chat(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid);
	void event_irc_dcc_send(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid);

	class IRC {
		public:
			irc_session_t * sess;

		protected:
			irc_callbacks_t callbacks;
			irc_ctx_t *ctx;

		protected:
			IRC();
			~IRC();

		protected:
			irc_session_t * CreateSession(irc_callbacks_t * callbacks);
			void DestroySession(irc_session_t * session);

		public:
			int Connect();
			int Run();
	};
}

#endif /* INCLUDE_TWDIRC_H */