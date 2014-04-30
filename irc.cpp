#include "main.h"
#include "irc.h"

namespace TWDevNet {
//	void IRC::event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx(session);
		event_dump (session, event, origin, params, count);

		char command[128];
		strcat(command, "IDENTIFY ");
		strcat(command, ctx->nickpass);
		irc_cmd_msg(session, "nickserv", command);
		irc_cmd_join (session, ctx->channel, 0);
	}

	void event_dump(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		char buf[512];
		unsigned int cnt;

		buf[0] = '\0';

		for ( cnt = 0; cnt < count; cnt++ ) {
			if ( cnt )
				strcat (buf, "|");

			strcat (buf, params[cnt]);
		}

		addlog ("Ev \"%s\", Or: \"%s\", Pa: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
	}

	void event_notice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_nick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_quit(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_part(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_mode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_topic(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_kick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_invite(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_umode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_ctcp_rep(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_ctcp_action(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_unknown(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
	}

	void event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		event_dump (session, event, origin, params, count);
		irc_cmd_user_mode (session, "+i");
		irc_cmd_me(session, params[0], "lurks");
	}

	void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		int result = 0;
		event_dump (session, event, origin, params, count);

		if ( !strcmp (params[1], "quit") )
			irc_cmd_quit (session, "of course, Master!");
		if ( strstr (params[1], "mode ") == params[1] )
			irc_cmd_channel_mode (session, params[0], params[1] + 5);
		if ( strstr (params[1], "nick ") == params[1] ) {
			result = irc_cmd_nick (session, params[1] + 5);
			switch(result) {
				case LIBIRC_RFC_ERR_NONICKNAMEGIVEN:
					irc_cmd_msg(session, origin, "Dumbass! Give me a nick to change to!");
				break;
				case LIBIRC_RFC_ERR_ERRONEUSNICKNAME:
					irc_cmd_msg(session, origin, "Erroneous Nick!");
				break;
				case LIBIRC_RFC_ERR_NICKNAMEINUSE:
					irc_cmd_msg(session, origin, "Nick in use!");
				break;
				case LIBIRC_RFC_ERR_NICKCOLLISION:
					irc_cmd_msg(session, origin, "Nick Collision!");
				break;
			}
		}
		if ( strstr (params[1], "whois ") == params[1] )
			irc_cmd_whois (session, params[1] + 5);
		if ( !strcmp (params[1], "help") )
			irc_cmd_msg (session, params[0], "help, dcc chat, dcc send, ctcp");
		if ( !strcmp (params[1], "topic") )
			irc_cmd_topic (session, params[0], 0);
		else if ( strstr (params[1], "topic ") == params[1] )
			irc_cmd_topic (session, params[0], params[1] + 6);

		printf ("'%s' said me (%s): %s\n", origin ? origin : "someone", params[0], params[1] );
	}

	void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		char nickbuf[128];

		if ( count != 2 )
			return;

		printf ("'%s' said in channel %s: %s\n", origin ? origin : "someone", params[0], params[1] );

		if ( !origin )
			return;

		irc_target_get_nick (origin, nickbuf, sizeof(nickbuf));

		if ( !strcmp (params[1], "~help") )
			irc_cmd_msg (session, params[0], "quit, help, dcc chat, dcc send, ctcp");

		if ( !strcmp (params[1], "~ctcp") ) {
			irc_cmd_ctcp_request (session, nickbuf, "PING 223");
			irc_cmd_ctcp_request (session, nickbuf, "FINGER");
			irc_cmd_ctcp_request (session, nickbuf, "VERSION");
			irc_cmd_ctcp_request (session, nickbuf, "TIME");
		}

		if ( !strcmp (params[1], "topic") )
			irc_cmd_topic (session, params[0], 0);
		else if ( strstr (params[1], "topic ") == params[1] )
			irc_cmd_topic (session, params[0], params[1] + 6);

	}

	void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count) {
	//	char buf[24];
	//	sprintf (buf, "%d", event);

	//	event_dump (session, buf, origin, params, count);
	}

	void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length) {
		static int count = 1;
		char buf[12];

		switch (status) {
			case LIBIRC_ERR_CLOSED:
				printf ("DCC %d: chat closed\n", id);
			break;

			case 0:
				if ( !data ) {
					printf ("DCC %d: chat connected\n", id);
					irc_dcc_msg	(session, id, "Hehe");
				}
				else {
					printf ("DCC %d: %s\n", id, data);
					sprintf (buf, "DCC [%d]: %d", id, count++);
					irc_dcc_msg	(session, id, buf);
				}
			break;

			default:
				printf ("DCC %d: error %s\n", id, irc_strerror(status));
			break;
		}
	}

	void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length) {
		if ( status == 0 && length == 0 ) {
			printf ("File sent successfully\n");

			if ( ctx )
				fclose ((FILE*) ctx);
		}
		else if ( status ) {
			printf ("File sent error: %d\n", status);

			if ( ctx )
				fclose ((FILE*) ctx);
		}
		else {
			if ( ctx )
				fwrite (data, 1, length, (FILE*) ctx);
			printf ("File sent progress: %d\n", length);
		}
	}

	void event_irc_dcc_chat(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid) {
		printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

		irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
	}

	void event_irc_dcc_send(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid) {
		FILE * fp;
		printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

		if ( (fp = fopen ("file", "wb")) == 0 )
			abort();

		irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
	}

	IRC::IRC() {
		memset (&callbacks, 0, sizeof(callbacks));
//		callbacks.event_connect = &IRC::event_connect;
		callbacks.event_connect = event_connect;
		callbacks.event_join = event_join;
		callbacks.event_nick = event_nick;
		callbacks.event_quit = event_quit;
		callbacks.event_part = event_part;
		callbacks.event_mode = event_mode;
		callbacks.event_topic = event_topic;
		callbacks.event_kick = event_kick;
		callbacks.event_channel = event_channel;
		callbacks.event_privmsg = event_privmsg;
		callbacks.event_notice = event_notice;
		callbacks.event_invite = event_invite;
		callbacks.event_umode = event_umode;
		callbacks.event_ctcp_rep = event_ctcp_rep;
		callbacks.event_ctcp_action = event_ctcp_action;
		callbacks.event_unknown = event_unknown;
		callbacks.event_numeric = event_numeric;

		callbacks.event_dcc_chat_req = event_irc_dcc_chat;
		callbacks.event_dcc_send_req = event_irc_dcc_send;
		addlog("%s", "Set up Callbacks complete");
		this->sess = irc_create_session (&callbacks);
		addlog("%s", "Session created");
	}
	IRC::~IRC() {
	}
	irc_session_t * IRC::CreateSession(irc_callbacks_t * callbacks) {
		return irc_create_session(callbacks);
	}
	void IRC::DestroySession(irc_session_t * session) {
		return irc_destroy_session(session);
	}
	int IRC::Connect() {
		if ( irc_connect(sess, ctx->server, ctx->port, ctx->password, ctx->nick, ctx->username, ctx->realname) ) {
			addlog("Could not connect: %s", irc_strerror (irc_errno(this->sess)));
			return 1;
		}
		return 0;
	}
	int IRC::Run() {
		if ( irc_run (this->sess) ) {
			addlog("Could not connect or IO error: (%d) %s", irc_errno(this->sess), irc_strerror (irc_errno(this->sess)));
			return 1;
		}
		return 0;
	}
}