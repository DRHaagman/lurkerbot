#include "main.h"
#include "irc.h"

namespace TWDevNet {
	IRC::IRC() {
		memset (&callbacks, 0, sizeof(callbacks));
		callbacks.event_connect = event_connect;
		callbacks.event_join = event_join;
		callbacks.event_nick = dump_event;
		callbacks.event_quit = dump_event;
		callbacks.event_part = dump_event;
		callbacks.event_mode = dump_event;
		callbacks.event_topic = dump_event;
		callbacks.event_kick = dump_event;
		callbacks.event_channel = event_channel;
		callbacks.event_privmsg = event_privmsg;
		callbacks.event_notice = notice_event;
		callbacks.event_invite = dump_event;
		callbacks.event_umode = dump_event;
		callbacks.event_ctcp_rep = dump_event;
		callbacks.event_ctcp_action = dump_event;
		callbacks.event_unknown = dump_event;
		callbacks.event_numeric = event_numeric;

		callbacks.event_dcc_chat_req = irc_event_dcc_chat;
		callbacks.event_dcc_send_req = irc_event_dcc_send;
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
//		irc_ctx_t *lctx = this->ctx;
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


	void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
		irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx ((irc_session_t*)session);
		dump_event ((irc_session_t*)session, event, origin, params, count);

		char command[128];
		strcat(command, "IDENTIFY ");
		strcat(command, ctx->nickpass);
		irc_cmd_msg((irc_session_t*)session, "nickserv", command);
		irc_cmd_join ((irc_session_t*)session, ctx->channel, 0);
	}
}