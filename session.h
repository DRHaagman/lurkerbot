#ifndef INCLUDE_TWDSESSION_H
#define INCLUDE_TWDSESSION_H
#include <iostream>
#include <string>

#include "irc.h"

namespace TWDevNet {
	class Session: public IRC {
		private:

		public:
			Session();
			~Session();

			void SetContext(irc_ctx_t * ctx);
	};
}

#endif /* INCLUDE_TWDSESSION_H */