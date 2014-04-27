#include "main.h"
#include "session.h"
#include <stdarg.h>

namespace TWDevNet {
	Session::Session() {
	}
	Session::~Session() {
	}

	void Session::SetContext(irc_ctx_t *ctx) {
		this->ctx = ctx;
		irc_set_ctx (s, ctx);
	}
}
