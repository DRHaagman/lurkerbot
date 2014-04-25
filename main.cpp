#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "libircclient.h"
#include "json.h"

#include "main.h"

void ReadConfig(const char * filename) {
	// Read the config file
	// Parse out the JSON and apply each server to an array of context structs.
	json_object* config;
	config = json_object_from_file(filename);

	addlog("%s", json_object_to_json_string(config));
/*
	servers[0].server = "irc.freenode.net";
	servers[0].port = 6667;
	servers[0].channel = (char*)"#twdev.bot"; //argv[3];
	servers[0].nick = (char*)"_lurker_"; //argv[2];
	servers[0].password = (char*)"";
	servers[0].username = "lurker";
	servers[0].realname = "Lurker Bot";
	servers[0].nickpass = "TvYxEw8M";
*/
}
void addlog(const char * fmt, ...) {
	FILE * fp;
	char buf[1024];
	va_list va_alist;

	va_start (va_alist, fmt);
#if defined (_WIN32)
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
#else
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
#endif
	va_end (va_alist);

	printf ("%s\n", buf);

	if ( (fp = fopen ("irctest.log", "ab")) != 0 ) {
		fprintf (fp, "%s\n", buf);
		fclose (fp);
	}
}

void dump_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
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

void notice_event(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event (session, event, origin, params, count);
}

void event_join(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event (session, event, origin, params, count);
	irc_cmd_user_mode (session, "+i");
	irc_cmd_me(session, params[0], "lurks");
}

void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	dump_event (session, event, origin, params, count);

	char command[128];
	strcat(command, "IDENTIFY ");
	strcat(command, ctx->nickpass);
	irc_cmd_msg(session, "nickserv", command);
	irc_cmd_join (session, ctx->channel, 0);
}

void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	int result = 0;
	dump_event (session, event, origin, params, count);

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

//	dump_event (session, buf, origin, params, count);
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

void irc_event_dcc_chat(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid) {
	printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

	irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
}

void irc_event_dcc_send(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid) {
	FILE * fp;
	printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

	if ( (fp = fopen ("file", "wb")) == 0 )
		abort();

	irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
}

int main (int argc, char **argv) {
	addlog("%s", "Starting");
	ReadConfig("configuration.json");
	addlog("%s", "Read Config complete");
	irc_callbacks_t	callbacks;
	irc_session_t * s;
/*
	if ( argc != 4 )
	{
		printf ("Usage: %s <server> <nick> <channel>\n", argv[0]);
		return 1;
	}
*/
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

	s = irc_create_session (&callbacks);
	addlog("%s", "Session created");

	if ( !s ) {
		addlog ("%s", "Could not create session\n");
		return 1;
	}

	addlog("%s", "Context set");
	irc_set_ctx (s, &servers[0]);
/*
	// If the port number is specified in the server string, use the port 0 so it gets parsed
	if ( strchr( argv[1], ':' ) != 0 )
		port = 0;

	// To handle the "SSL certificate verify failed" from command line we allow passing ## in front
	// of the server name, and in this case tell libircclient not to verify the cert
	if ( argv[1][0] == '#' && argv[1][1] == '#' )
	{
		// Skip the first character as libircclient needs only one # for SSL support, i.e. #irc.freenode.net
		argv[1]++;

		irc_option_set( s, LIBIRC_OPTION_SSL_NO_VERIFY );
	}
*/
	// Initiate the IRC server connection
	addlog("%s%s", "Connecting to ", servers[0].server);
	if ( irc_connect(s, servers[0].server, servers[0].port, servers[0].password, servers[0].nick, servers[0].username, servers[0].realname) ) {
		addlog("Could not connect: %s", irc_strerror (irc_errno(s)));
		return 1;
	}
	addlog("%s%s", "Connection made to ", servers[0].server);

	// and run into forever loop, generating events
	addlog("%s", "Running event pump...");
	if ( irc_run (s) ) {
		addlog("Could not connect or IO error: (%d) %s", irc_errno(s), irc_strerror (irc_errno(s)));
	}

	return 1;
}
