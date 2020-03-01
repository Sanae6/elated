#ifndef WINDOW_HEAD
#define WINDOW_HEAD
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Scripting.hpp"

namespace win {
	extern GLFWwindow* _window;
	//vector<
	bool shouldClose();
	void setShouldClose(bool close);
	int initialize();
	void loop();
	void quit();
};
#endif