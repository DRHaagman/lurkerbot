#include "main.h"
#include "configurator.h"
#include "session.h"

namespace TWDevNet {
	vector<Session *> s;

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
	string servername;

	addlog("Starting");
	Configurator *config = new Configurator("configuration.json");
	if (!config->valid)
		return (!config->valid);
	addlog("Read Config complete");

	for (int x = 0;x < config->servercount;x++) {
		// TODO: thread the loop so that s[x]->Run() will execute for every server.
		servername = config->servers[x]->name;
		Session *tmpS = new Session(config->servers[x]);
		s.push_back(tmpS);
		if ( !s[x]->sess ) {
			addlog ("Could not create session for server %s\n", servername.c_str());
			return 1;
		}
		else
			addlog("Session created with context set");
		// To handle the "SSL certificate verify failed" from command line we allow passing ## in front
		// of the server name, and in this case tell libircclient not to verify the cert
		if (servername.c_str()[0] == '#' && servername.c_str()[1] == '#' ) {
			// Skip the first character as libircclient needs only one # for SSL support, i.e. #irc.freenode.net
			irc_option_set(s[x]->sess, LIBIRC_OPTION_SSL_NO_VERIFY );
		}

		// Initiate the IRC server connection
		addlog("Connecting to %s", servername.c_str());
		if (s[x]->Connect()) {
			addlog("Connection made to %s", servername.c_str());

			// and run into forever loop, generating events
			addlog("Running event pump for %s", servername.c_str());
			if (!s[x]->Run()) {
				addlog("Teminated %s", servername.c_str());
				return 1;
			}
		}
		else
			addlog("Connection failed for %s", servername.c_str());
	}
	addlog("Concluded for %s", servername.c_str());
	return 0;
}
