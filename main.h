#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

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

irc_ctx_t servers[255];

void addlog(const char * fmt, ...);

#endif // INCLUDE_MAIN_H