#include "main.h"

namespace TWDevNet {
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

}
using namespace TWDevNet;

int main (int argc, char **argv) {
	Configurator config;	//	config = Configurator::Configurator("configuration.json");

	addlog("%s", "Starting");
	config.ReadConfig("configuration.json");
	addlog("%s", "Read Config complete");

	Session s;

	if ( !s.s ) {
		addlog ("%s", "Could not create session\n");
		return 1;
	}

	addlog("%s", "Context set");
	irc_set_ctx (s, &config.servers[0]);
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
	addlog("%s%s", "Connecting to ", config.servers[0].server);
	if ( irc_connect(s, config.servers[0].server, config.servers[0].port, config.servers[0].password, config.servers[0].nick, config.servers[0].username, config.servers[0].realname) ) {
		addlog("Could not connect: %s", irc_strerror (irc_errno(s)));
		return 1;
	}
	addlog("%s%s", "Connection made to ", config.servers[0].server);

	// and run into forever loop, generating events
	addlog("%s", "Running event pump...");
	if ( irc_run (s) ) {
		addlog("Could not connect or IO error: (%d) %s", irc_errno(s), irc_strerror (irc_errno(s)));
	}

	return 1;
}
