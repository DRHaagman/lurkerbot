#ifndef INCLUDE_TWDSESSION_H
#define INCLUDE_TWDSESSION_H
#include <iostream>
#include <string>

#include "irc.h"

namespace TWDevNet {
	class Session: public IRC {
		private:
			void SetContext(irc_ctx_t * ctx);

		public:
//			Session();
			Session(irc_ctx_t * ctx);
			~Session();

			irc_ctx_t GetContext();

		public: // Event Handlers
//			void EventChannel(irc_callback_params_t * Params);
			int FindChannelIdx(string Channel);
	};
}

#endif /* INCLUDE_TWDSESSION_H */