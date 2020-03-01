#include "Files.hpp"

namespace fs {
	const char* readFileText(const char* location) {
		std::string text;
		std::string line;
		std::ifstream file(location);
		while (getline(file, line)) {
			text += line;
			text += "\n";
		}
		file.close();
		return text.c_str();
	}
	std::vector<unsigned char> readFileBinary(const char* location) {
		std::ifstream inp(location, std::ios::binary);
		std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inp), {});
		return buffer;
	}
}