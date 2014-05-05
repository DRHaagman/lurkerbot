#ifndef INCLUDE_TWDCONFIGURATOR_H
#define INCLUDE_TWDCONFIGURATOR_H

#include <iostream>
#include <string>

#include "irc.h"
#include "json.h"

namespace TWDevNet {
	class Configurator {
		public:
			int servercount;
			irc_ctx_t servers[255];
			int valid;

		public:
			Configurator();
			Configurator(const char *filename);
			~Configurator();

		private:
			string FetchAndUnquoteJSElement(json_object *jsobject, string ElementName);
			int FetchIntJSElement(json_object *jsobject, string ElementName);
		public:
			int ReadConfig(const char *filename);
	};
}

#endif /* INCLUDE_TWDCONFIGURATOR_H */