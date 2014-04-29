#ifndef INCLUDE_TWDIRC_H
#define INCLUDE_TWDIRC_H
#include <iostream>
#include <string>

#include "libircclient.h"

namespace TWDevNet {
	class IRC {
		public:
			irc_callbacks_t callbacks;
			irc_session_t * s;

		public:
			irc_ctx_t *ctx;

		public:
			IRC();
			~IRC();

		public:
			irc_session_t * CreateSession(irc_callbacks_t * callbacks);
			void DestroySession(irc_session_t * session);
			int Connect();
			int Run();
	};
}

#endif /* INCLUDE_TWDIRC_H */