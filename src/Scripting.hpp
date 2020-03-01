#ifndef SCRIPTING_HEAD
#define SCRIPTING_HEAD
#include <duktape.h>
#include <vector>
#include "Files.hpp"
namespace scr {
	extern duk_context* _context;
	extern bool _init;
	void evalFile(const char* filename);
	int initialize();
	void quit();
}
#endif