#include "main.h"
#include "session.h"
#include <stdarg.h>

namespace TWDevNet {
	Session::Session() {
	}
	Session::Session(irc_ctx_t *ctx) {
		SetContext(ctx);
	}
	Session::~Session() {
	}

	/* Pointer to the original passed in */
	void Session::SetContext(irc_ctx_t *ctx) {
		this->ctx = ctx;
		irc_set_ctx (this->sess, ctx);
	}
	/* Copy of ctx returned for protection */
	irc_ctx_t Session::GetContext() {
		return *this->ctx;
	}
	int Session::FindChannelIdx(string Channel) {
		irc_ctx_t ctx = *this->ctx;  // Get a local COPY of the context.
		// Loop through the context channels to find the specified one and return it's index.
		for (int x = 0;x < ctx.channelcount;x++) {
			if (ctx.channels[x]->name == Channel)
				return x;
		}
		return -1;
	}

	void Session::EventChannel(irc_callback_params_t * Params) {

	}
}
