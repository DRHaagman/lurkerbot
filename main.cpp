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
	Session *s[255];
	addlog("Starting");
	Configurator *config = new Configurator("configuration.json");
//	int retval = config.ReadConfig("configuration.json");
	if (!config->valid)
		return (!config->valid);
	addlog("Read Config complete");

	for (int x = 0;x < config->servercount;x++) {
		// TODO: thread the loop so that s[x]->Run() will execute for every server.
		Session *tmpS = new Session;
		s[x] = tmpS;

		if ( !s[x]->sess ) {
			addlog ("Could not create session\n");
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
		s[x]->SetContext(&config->servers[0]);
		addlog("Context set");

		// Initiate the IRC server connection
		addlog("Connecting to %s", s[x]->GetContext().server.c_str());
		if (s[x]->Connect()) {
			addlog("Connection made to %s", s[x]->GetContext().server.c_str());

			// and run into forever loop, generating events
			addlog("Running event pump...");
			if (!s[x]->Run()) {
				addlog("Teminated");
				return 1;
			}
		}
		else
			addlog("Connection failed");
	}
	addlog("Concluded");
	return 0;
}
