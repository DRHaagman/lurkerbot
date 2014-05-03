#ifndef INCLUDE_TWDCONFIGURATOR_H
#define INCLUDE_TWDCONFIGURATOR_H

#include <iostream>
#include <string>

#include "irc.h"
#include "json.h"

namespace TWDevNet {
	class Configurator {
		public:
			irc_ctx_t servers[255];

		public:
			Configurator();
			Configurator(const char *filename);
			~Configurator();

//		protected:
			int ReadConfig(const char *filename);
	};
}

#endif /* INCLUDE_TWDCONFIGURATOR_H */