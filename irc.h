#ifndef INCLUDE_TWDIRC_H
#define INCLUDE_TWDIRC_H
#include <iostream>
#include <string>

#include "libircclient.h"

namespace TWDevNet {
	class IRC {
		public:
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
		public:
			IRC();
			~IRC();
		protected:
			irc_session_t * CreateSession(irc_callbacks_t * callbacks);
			void DestroySession(irc_session_t * session);
	};
}

#endif /* INCLUDE_TWDIRC_H */