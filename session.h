#ifndef INCLUDE_TWDSESSION_H
#define INCLUDE_TWDSESSION_H
#include <iostream>
#include <string>

#include "irc.h"

namespace TWDevNet {
	class Session: public IRC {
		public:
			Session();
			~Session();
	};
}

#endif /* INCLUDE_TWDSESSION_H */