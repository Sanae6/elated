#ifndef FILES_HEAD
#define FILES_HEAD
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace fs {
	char* resolveLocation();//work on sandboxing (prevent users from creating malicious content that escapes the data folder)
	const char* readFileText(const char* location);
}

#endif