#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "libircclient.h"
#include "json.h"

#include "configurator.h"
#include "session.h"

namespace TWDevNet {
	void addlog(const char * fmt, ...);
}
#endif // INCLUDE_MAIN_H