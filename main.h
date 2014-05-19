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
		string name;                  // Name of the Action
		string aliases;               // Aliases
		string action;                // Action to perform
		string command;               // Command to parse
		string type;                  // Type of command
		string help;                  // Help text to display when "Help" action is triggered.
		bool channelaware;            // Does the action automatically relate to the active channel name when the action is triggered?
	} irc_chan_act_t;

	typedef struct {
		string name;                  // Name of the Channel.
		string flags;                 // Nick flags for the channel.
		string actiontrigger;         // Trigger character to parse for the actions.
		int actioncount;              // Number of actions defined.
		irc_chan_act_t *actions[255]; // Pointer to the array of actions.
	} irc_chan_t;

	typedef struct {
		string realname;              // "Real Name" for the nick.
		string name;                  // Name of the Server
		string type;                  // Type of server
		string server;                // Server host (DN / IP)
		unsigned short port;          // Server connect port
		string username;              // Server user-account
		string password;              // Server user account password
		string nick;                  // Nickname
		string nickpass;              // Password for authentication to services.
		irc_chan_act_t *actions[255]; // Pointer to the array of actions - These are independant of channels.
		int channelcount;             // Number of channels defined
		irc_chan_t *channels[255];    // Pointer to the array of channels.
		int sessionno;
	} irc_ctx_t;

	typedef struct {
		irc_session_t * session;
		const char * event;
		const char * origin;
		const char ** params;
		unsigned int count;
	} irc_callback_params_t;

	void addlog(const char * fmt, ...);
}
#endif // INCLUDE_MAIN_H