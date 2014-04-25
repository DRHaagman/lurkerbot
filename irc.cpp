#include "irc.h"

namespace TWDevNet {
	IRC::IRC() {
	}
	IRC::~IRC() {
	}
	irc_session_t * IRC::CreateSession(irc_callbacks_t * callbacks) {
		return irc_create_session(callbacks);
	}
	void IRC::DestroySession(irc_session_t * session) {
		return irc_destroy_session(session);
	}
}