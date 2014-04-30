#include "main.h"
#include "session.h"
#include <stdarg.h>

namespace TWDevNet {
	Session::Session() {
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
}
