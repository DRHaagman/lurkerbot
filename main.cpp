#include "main.h"
#include "configurator.h"
#include "session.h"

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
	Configurator config;

	addlog("%s", "Starting");
	int retval = config.ReadConfig("configuration.json");
	if (retval)
		return retval;
	addlog("%s", "Read Config complete");

	Session *s = new Session();

	if ( !s->sess ) {
		addlog ("%s", "Could not create session\n");
		return 1;
	}
/*
	// To handle the "SSL certificate verify failed" from command line we allow passing ## in front
	// of the server name, and in this case tell libircclient not to verify the cert
	if ( config.servers[0][0] == '#' && config.servers[0][1] == '#' ) {
		// Skip the first character as libircclient needs only one # for SSL support, i.e. #irc.freenode.net
		irc_option_set( config, LIBIRC_OPTION_SSL_NO_VERIFY );
	}
*/
	s->SetContext(&config.servers[0]);
	addlog("%s", "Context set");

	// Initiate the IRC server connection
	addlog("%s%s", "Connecting to ", s->GetContext().server);
	if (s->Connect()) {
		addlog("%s%s", "Connection made to ", s->GetContext().server);

		// and run into forever loop, generating events
		addlog("%s", "Running event pump...");
		if (!s->Run()) {
			return 1;
		}
	}
	return 0;
}
