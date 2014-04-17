#include <stdio.h>
#include <string.h>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

static void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {

}

static void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count) {

}

int main(int argc, char **argv)
{
	// The IRC callbacks structure
	irc_callbacks_t callbacks;

	// Init it
	memset ( &callbacks, 0, sizeof(callbacks) );

	// Set up the mandatory events
	callbacks.event_connect = event_connect;
	callbacks.event_numeric = event_numeric;

	// Set up the rest of events

	// Now create the session
	irc_session_t * session = irc_create_session( &callbacks );

	if ( !session ) {
		// Handle the error
		printf("FAILED TO CREATE SESSION!\n");
		return 1;
	}
	irc_option_set( session, LIBIRC_OPTION_STRIPNICKS );
	// Connect to a regular IRC server
	if ( irc_connect (session, "irc.freenode.net", 6667, 0, "_lurker_", "lurkerbot", "Lurker Bot" ) ) {
		// Handle the error: irc_strerror() and irc_errno()
		printf("FAILED TO CONNECT TO SERVER!\n");
	}
	if ( irc_run (session) ) {
		// Either the connection to the server could not be established or terminated. See irc_errno()
		printf("CONNECTION TERMINATED!\n");
	}
	return 0;
}

